/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/

#pragma once

#define MAX_DURATION (120) // le nombre de second maximum pour loader un fichier audio

#include <JuceHeader.h>
#include "../Utils/StateParameters.h"
#include "ThumbnailComponent.h"

// https://github.com/davedema/GranularSynth/tree/master/Grana

class AudioFileLoader : public juce::Component
{
public:
	AudioFileLoader(StateParameters*, ThumbnailComponent*);
	~AudioFileLoader();
	void AudioFileLoader::loadAudio(juce::File&);
	void loadFile();
	void loadFile(const juce::String&);

	juce::AudioFormatManager* getFormatManager();

	// pour le spectrumcomponent



private:

	void showErrorWindow(juce::String message);

	ThumbnailComponent* thumbnailComponent;
	juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
	std::unique_ptr<juce::AudioFormatReaderSource>* readerSource;
	StateParameters* stateParams;

	// pour le spectrumcomponent



};