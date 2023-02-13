/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StateSaver.h"

class AudioFileLoader {
public:
	AudioFileLoader(StateSaver*);
	~AudioFileLoader();
	void loadFile();

private:
	juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
	//juce::AudioFormatReader* reader{ nullptr };
	std::unique_ptr<juce::AudioFormatReaderSource>* readerSource;
	juce::AudioBuffer<float>* buffer;
	StateSaver* stateSaver;


};