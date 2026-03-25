
#include "AudioFileLoader.h"

namespace particules
{
    AudioFileLoader::AudioFileLoader() : sampleRate{0.0} { formatManager.registerBasicFormats(); }

    void AudioFileLoader::loadFile(AudioLoadedCallback onAudioLoaded)
    {
        if(!chooser)
            chooser = std::make_unique<juce::FileChooser>(
                "Select an audio file.", juce::File{}, formatManager.getWildcardForAllFormats());
        int flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(flags, [this, onAudioLoaded](const juce::FileChooser& resultChooser) {
            juce::File file = resultChooser.getResult();
            if(currentFile == file)
                return;

            processLoadingFile(file, onAudioLoaded);
        });
    }

    void AudioFileLoader::loadFile(const juce::String& path, AudioLoadedCallback onAudioLoaded)
    {
        juce::File file(path);
        if(currentFile == file)
            return;

        processLoadingFile(file, onAudioLoaded);
    }

    void AudioFileLoader::processLoadingFile(juce::File& file, AudioLoadedCallback onAudioLoaded)
    {
        bool ok = false;
        if(file.existsAsFile())
        {
            AudioBuffer bufferOut;
            ok = loadAudioFromFile(file, bufferOut);
            if(ok)
            {
                setCurrentFile(file);
                onAudioLoaded(bufferOut);
            }
            
        }
    }

    bool AudioFileLoader::loadAudioFromFile(juce::File& file, AudioBuffer& bufferOut)
    {
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

        if(!reader)
            return false;

        // si le reader de fichier lit un fichier de plus de MaxDuration alors on annule tout
        const double fileDuration = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
        if(fileDuration >= MAX_DURATION)
        {
            showErrorWindow("The file duration must not exceed " + (juce::String)MAX_DURATION+ " seconds."
                            + "/nYour file is currently " + juce::String(fileDuration, 2) + " seconds.");
            return false;
        }

        //const double targetSampleRate = paramsView.getSampleRate();
        const double targetSampleRate = sampleRate;
        if(targetSampleRate <= 0.0)
        {
            showErrorWindow("invalid sample rate : targetSampleRate <= 0");
            return false;
        }

        const int numSamples = (int)reader->lengthInSamples;
        const int inputNumChannels = reader->numChannels;

        if(file.getSize() > MAX_FILE_SIZE)
        {
            showErrorWindow("File too large to process (" + juce::String(file.getSize() / 2 * (1024 * 1024)) + " MB estimated)");
            return false;
        }

        double ratio = reader->sampleRate / targetSampleRate;

        juce::AudioBuffer<float> tempBuffer(inputNumChannels, numSamples);
        if(!reader->read(&tempBuffer, 0, numSamples, 0, true, true))
        {
            showErrorWindow("Failed to read audio data from the temporary buffer");
            return false;
        }

        const int resampledSamples = static_cast<int>(numSamples / ratio);

        juce::AudioBuffer<float> resampledBuffer(inputNumChannels, resampledSamples);

        juce::LagrangeInterpolator resampler;

        for(int channel = 0; channel < resampledBuffer.getNumChannels(); ++channel)
        {
            resampler.reset();
            resampler.process(
                ratio, tempBuffer.getReadPointer(channel), resampledBuffer.getWritePointer(channel), resampledSamples);
        }

        if(resampledBuffer.getNumSamples() == 0)
        {
            showErrorWindow("Invalid sample source created : resampledBuffer.getNumSamples() == 0");
            return false;
        }

        bufferOut = channelMixer.downmix(resampledBuffer);

        return true;
    }

    void AudioFileLoader::init(double sr, int numCh) noexcept
    {
        if(sr >= 0.0)
            sampleRate = sr;
        channelMixer.setTargetChannel(numCh);
    }

    juce::AudioFormatManager& AudioFileLoader::getFormatManager() { return formatManager; }

    void AudioFileLoader::showErrorWindow(const juce::String& message)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", message, "OK");
    }
}