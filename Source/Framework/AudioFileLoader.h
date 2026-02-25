/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/

#pragma once
#include <juce_audio_formats/juce_audio_formats.h>

#include "Core.h"

class ParameterView;
class AudioFileLoader
{
public:
	AudioFileLoader(ParameterView&);
	~AudioFileLoader() = default;

	void loadFile(std::function<void(juce::File, bool)> completion);
	void loadFile(const juce::String&, std::function<void(juce::File, bool)> completion);

	bool loadAudio(juce::File&);


	juce::AudioFormatManager& getFormatManager();

private:
	static constexpr uint64_t maxFileSize = Param::MaxFileSize;
	static constexpr uint8_t maxDuration = Param::MaxDuration;


	void showErrorWindow(juce::String message);
	std::unique_ptr<juce::FileChooser> chooser;

	ParameterView& paramsView;
	juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
};
//void setOnFileLoadedCallBack(std::function<void(bool)> foo);
//std::function<void(bool)> onFileLoaded;