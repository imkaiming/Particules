/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/

#pragma once


#include <functional>
#include "../Frame/ThumbnailComponent.h"

class ParameterView;
class AudioFileLoader
{
public:
	AudioFileLoader(ParameterView&, ThumbnailComponent&);
	~AudioFileLoader() = default;

	bool loadAudio(juce::File&);
	void loadFile();
	void loadFile(const juce::String&);
	

	void setOnFileLoadedCallBack(std::function<void(bool)> foo);
	juce::AudioFormatManager& getFormatManager();

private:
	static constexpr uint64_t maxFileSize = Param::MaxFileSize;
	static constexpr uint8_t maxDuration = Param::MaxDuration;

	std::function<void(bool)> onFileLoaded;

	void showErrorWindow(juce::String message);
	std::unique_ptr<juce::FileChooser> chooser;

	ParameterView& paramsView;
	ThumbnailComponent& thumbnailComponent;
	juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
};