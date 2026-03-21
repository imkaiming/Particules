/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include "ChannelMixer.h"
#include "Core.h"

class AudioFileLoader
{
public:
    AudioFileLoader();
    ~AudioFileLoader() = default;

    void loadFile(AudioLoadedCallback);

    void loadFile(const juce::String&, AudioLoadedCallback);

    void processLoadingFile(juce::File&, AudioLoadedCallback);

    bool loadAudioFromFile(juce::File&, AudioBuffer&);

    void init(double, int) noexcept;
    juce::AudioFormatManager& getFormatManager();
    const juce::File& getCurrentFile() const noexcept { return currentFile; };
    void setCurrentFile(juce::File& f) noexcept { currentFile = f; };

private:
    static constexpr int maxFileSize = Param::MaxFileSize;
    static constexpr double maxDuration = Param::MaxDuration;
    void showErrorWindow(const juce::String&);

    double sampleRate;

    ChannelMixer channelMixer;
    juce::File currentFile;
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileLoader)
};
//void setOnFileLoadedCallBack(std::function<void(bool)> foo);
//std::function<void(bool)> onFileLoaded;