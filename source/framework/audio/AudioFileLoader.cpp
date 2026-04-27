
#include "AudioFileLoader.h"

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    AudioFileLoader::AudioFileLoader() : sampleRate{0.0}, formatManager{}, targetChannels{0}
    {
        formatManager.registerBasicFormats();
    }

    void AudioFileLoader::loadFile(AudioLoadedCallback onAudioLoaded, const juce::File& currentFile)
    {
        if(!chooser)
            chooser = std::make_unique<juce::FileChooser>(
                "Select an audio file.", juce::File{}, formatManager.getWildcardForAllFormats());
        const int flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(flags, [this, onAudioLoaded, currentFile](const juce::FileChooser& resultChooser) {
            juce::File file = resultChooser.getResult();
            if(currentFile == file)
                return;

            std::thread([this, file, onAudioLoaded]() { processLoadingFile(file, onAudioLoaded); }).detach();
            //threadPool.addJob([this, file, onAudioLoaded]() { processLoadingFile(file, onAudioLoaded); });
        });
    }

    void AudioFileLoader::loadFile(const str& path, AudioLoadedCallback onAudioLoaded, const juce::File& currentFile)
    {
        juce::File file(path);
        if(currentFile == file)
            return;

        std::thread([this, file, onAudioLoaded]() { processLoadingFile(file, onAudioLoaded); }).detach();
        //threadPool.addJob([this, file, onAudioLoaded]() { processLoadingFile(file, onAudioLoaded); }); // very weird Heisenbug ????
    }

    void AudioFileLoader::processLoadingFile(const juce::File& file, AudioLoadedCallback onAudioLoaded)
    {
        //DBG("THREAD POOL");
        if(file.existsAsFile())
        {
            std::unique_ptr<AudioBuffer> finalBufferPtr = loadAudioFromFile(file);
            if(finalBufferPtr != nullptr)
                onAudioLoaded(std::move(finalBufferPtr), file);
        }
    }

    std::unique_ptr<AudioBuffer> AudioFileLoader::loadAudioFromFile(const juce::File& file)
    {
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

        if(!reader)
            return nullptr;

        // 1. security step
        const double fileDuration = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
        if(fileDuration >= params::maxFileDuration)
        {
            showErrorWindow("The file duration must not exceed " + str(params::maxFileDuration)
                            + " seconds.\nYour file is currently " + str(fileDuration, 2) + " seconds.");
            return nullptr;
        }

        const double targetSampleRate = sampleRate;
        if(targetSampleRate <= 0.0)
        {
            showErrorWindow("Invalid sample rate : targetSampleRate <= 0");
            return nullptr;
        }

        if(file.getSize() > params::maxFileSize)
        {
            showErrorWindow("File too large to process (" + str(file.getSize() / (1024 * 1024)) + " MB estimated)");
            return nullptr;
        }

        // 2. loading file
        const int numSamples = static_cast<int>(reader->lengthInSamples);
        const int inputNumChannels = reader->numChannels;

        AudioBuffer readBuffer(inputNumChannels, numSamples);
        if(!reader->read(&readBuffer, 0, numSamples, 0, true, true))
        {
            showErrorWindow("Failed to read audio data from the file");
            return nullptr;
        }

        // 3. downmix
        AudioBuffer mixedBuffer = channelMixer.downmix(readBuffer);

        // 4. ratio
        const double ratio = reader->sampleRate / targetSampleRate;
        const int resampledSamples = static_cast<int>(numSamples / ratio);

        if(resampledSamples <= 0)
        {
            showErrorWindow("Invalid sample source created : resampledSamples <= 0");
            return nullptr;
        }

        // 5. heap allocation + added a guard Sample
        const int mixedChannels = mixedBuffer.getNumChannels();
        std::unique_ptr<juce::AudioBuffer<float>> finalBuffer =
            std::make_unique<juce::AudioBuffer<float>>(mixedChannels, resampledSamples + 1);

        // 6. resampling is bypassed if sample rate is identical
        if(std::abs(ratio - 1.0) < 0.00001)
        {
            for(int ch = 0; ch < mixedChannels; ++ch)
            {
                finalBuffer->copyFrom(ch, 0, mixedBuffer, ch, 0, resampledSamples);
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
                finalBuffer->getWritePointer(ch)[resampledSamples] = finalBuffer->getReadPointer(ch)[0]; // guard sample
            }
        }

        return finalBuffer;
    }

    void AudioFileLoader::init(double sr, int numCh) noexcept
    {
        if(sr >= 0.0)
            sampleRate = sr;
        channelMixer.setTargetChannel(numCh);
    }

    void AudioFileLoader::setSampleRate(double sr) noexcept
    {
        if(sr > 0)
            sampleRate = sr;
    }
    void AudioFileLoader::setNumTargetChannels(int ch) noexcept
    {
        assert(ch > 0);
        targetChannels = ch;
        channelMixer.setTargetChannel(ch);
    }

    juce::AudioFormatManager& AudioFileLoader::getFormatManager() { return formatManager; }

    void AudioFileLoader::showErrorWindow(const str& message)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", message, "OK");
    }
}