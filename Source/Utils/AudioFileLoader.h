/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "StateSaver.h"

class AudioFileLoader {
public:
	AudioFileLoader();
	~AudioFileLoader();
	void openFile(juce::AudioBuffer<float>*);
	void loadAudio(juce::File, juce::AudioBuffer<float>*);

private:
	//juce::File* audioFile;
	juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource; // fichier d'origine
	/*juce::AudioBuffer<float>* buffer;*/ 
	//StateSaver* stateSaver;
};