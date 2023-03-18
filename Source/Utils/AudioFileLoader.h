/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/StateParameters.h"

// https://github.com/davedema/GranularSynth/tree/master/Grana

class AudioFileLoader {
public:
	AudioFileLoader(StateParameters*);
	~AudioFileLoader();
	void AudioFileLoader::loadAudio(juce::File&);
	void loadFile();
	void loadFile(const juce::String&);
	void unloadFile();
	juce::AudioBuffer<float>* getAudioBuffer() const;

private:
	//static AudioFileLoader* instance;
	juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
	//juce::AudioFormatReader* reader;
	std::unique_ptr<juce::AudioFormatReaderSource>* readerSource;
	juce::AudioBuffer<float>* buffer; 
	StateParameters* stateParams;

};