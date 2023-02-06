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
	AudioFileLoader();
	~AudioFileLoader();
	void init(StateSaver*);
	void loadAudioFile();

private:
	juce::File audioFile;
	StateSaver* stateSaver;

	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	//TransportState state;
};
