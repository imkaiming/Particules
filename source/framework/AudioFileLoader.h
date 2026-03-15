/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include "Core.h"

using AudioLoadedCallback = std::function<void(std::shared_ptr<const AudioBuffer>)>;

class AudioFileLoader
{
public:
    AudioFileLoader();
    ~AudioFileLoader() = default;

    void loadFile(AudioLoadedCallback);

    void loadFile(const juce::String&, AudioLoadedCallback);

    void processLoadingFile(juce::File&, AudioLoadedCallback);

    bool loadAudioFromFile(juce::File&, std::shared_ptr<const AudioBuffer>&);

    void init(double) noexcept;
    juce::AudioFormatManager& getFormatManager();
    const juce::File& getCurrentFile() const noexcept { return currentFile; };
    void setCurrentFile(juce::File& f) noexcept { currentFile = f; };

private:
    static constexpr int maxFileSize = Param::MaxFileSize;
    static constexpr double maxDuration = Param::MaxDuration;
    void showErrorWindow(juce::String message);

    double sampleRate;

    juce::File currentFile;
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileLoader)
};
//void setOnFileLoadedCallBack(std::function<void(bool)> foo);
//std::function<void(bool)> onFileLoaded;