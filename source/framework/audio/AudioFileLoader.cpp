#include "AudioFileLoader.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    namespace
    {
        constexpr double ratioEpsilon = 0.00001;

        std::unique_ptr<AudioBuffer> loadAudioBufferFromFile(
            const juce::File& file, juce::AudioFormatManager& formatManager, ChannelMixer& channelMixer, double targetSampleRate)
        {
            std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

            if(!reader)
                return nullptr;

            // Validation
            const double fileDuration = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;

            if(fileDuration >= params::maxFileDuration)
            {
                return nullptr;
            }

            if(targetSampleRate <= 0.0)
            {
                return nullptr;
            }

            if(file.getSize() > params::maxFileSize)
            {
                return nullptr;
            }

            // Read file
            const int numSamples = static_cast<int>(reader->lengthInSamples);

            const int inputNumChannels = static_cast<int>(reader->numChannels);

            AudioBuffer readBuffer(inputNumChannels, numSamples);

            if(!reader->read(&readBuffer, 0, numSamples, 0, true, true))
            {
                return nullptr;
            }

            // Downmix
            AudioBuffer mixedBuffer = channelMixer.downmix(readBuffer);

            // Resampling
            const double ratio = reader->sampleRate / targetSampleRate;

            const int resampledSamples = static_cast<int>(numSamples / ratio);

            if(resampledSamples <= 0)
            {
                return nullptr;
            }

            // Final buffer (+1 guard sample)
            const int mixedChannels = mixedBuffer.getNumChannels();

            auto finalBuffer = std::make_unique<juce::AudioBuffer<float>>(mixedChannels, resampledSamples + 1);

            // Copy / Resample
            if(std::abs(ratio - 1.0) < ratioEpsilon)
            {
                for(int ch = 0; ch < mixedChannels; ++ch)
                {
                    finalBuffer->copyFrom(ch, 0, mixedBuffer, ch, 0, resampledSamples);

                    // guard sample
                    finalBuffer->getWritePointer(ch)[resampledSamples] = finalBuffer->getReadPointer(ch)[0];
                }
            }
            else
            {
                juce::WindowedSincInterpolator resampler;

                for(int ch = 0; ch < mixedChannels; ++ch)
                {
                    resampler.reset();

                    resampler.process(ratio, mixedBuffer.getReadPointer(ch), finalBuffer->getWritePointer(ch), resampledSamples);

                    // guard sample
                    finalBuffer->getWritePointer(ch)[resampledSamples] = finalBuffer->getReadPointer(ch)[0];
                }
            }

            return finalBuffer;
        }
    }

    //==================================================================

    AudioFileLoader::AudioFileLoader() : sampleRate(0.0), targetChannels(0) { formatManager.registerBasicFormats(); }

    AudioFileLoader::~AudioFileLoader() { threadPool.removeAllJobs(true, 1000); }

    //==================================================================

    void AudioFileLoader::loadFile(AudioLoadedCallback onAudioLoaded, const juce::File& currentFile)
    {
        if(!chooser)
        {
            chooser = std::make_unique<juce::FileChooser>(
                "Select an audio file.", juce::File{}, formatManager.getWildcardForAllFormats());
        }

        const int flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(flags, [this, onAudioLoaded, currentFile](const juce::FileChooser& resultChooser) {
            const juce::File file = resultChooser.getResult();

            if(file == juce::File{})
                return;

            if(file == currentFile)
                return;

            launchLoadingJob(file, onAudioLoaded);
        });
    }

    void AudioFileLoader::loadFile(const str& path, AudioLoadedCallback onAudioLoaded, const juce::File& currentFile)
    {
        const juce::File file(path);

        if(!file.existsAsFile())
            return;

        if(file == currentFile)
            return;

        launchLoadingJob(file, onAudioLoaded);
    }


    void AudioFileLoader::launchLoadingJob(const juce::File& file, AudioLoadedCallback onAudioLoaded)
    {
        if(loading.exchange(true))
            return;

        const double targetSampleRate = sampleRate;

        threadPool.addJob([this, file, cb = std::move(onAudioLoaded), targetSampleRate]() mutable {
            auto buffer = loadAudioBufferFromFile(file, formatManager, channelMixer, targetSampleRate);

            juce::MessageManager::callAsync([this, file, cb = std::move(cb), buffer = std::move(buffer)]() mutable {
                loading = false;

                if(!buffer)
                {
                    showErrorWindow("Failed to load audio file.");
                    return;
                }

                cb(std::move(buffer), file);
            });
        });
    }

    void AudioFileLoader::init(double sr, int numCh) noexcept
    {
        if(sr > 0.0)
        {
            sampleRate = sr;
        }

        setNumTargetChannels(numCh);
    }

    void AudioFileLoader::setSampleRate(double sr) noexcept
    {
        if(sr > 0.0)
        {
            sampleRate = sr;
        }
    }

    void AudioFileLoader::setNumTargetChannels(int ch) noexcept
    {
        jassert(ch > 0);

        targetChannels = ch;
        channelMixer.setTargetChannel(ch);
    }

    juce::AudioFormatManager& AudioFileLoader::getFormatManager() { return formatManager; }

    void AudioFileLoader::showErrorWindow(const str& message)
    {
        juce::MessageManager::callAsync(
            [message]() { juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", message, "OK"); });
    }


} 