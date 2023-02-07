/*
  ==============================================================================

	AudioFileLoader.h
	Created: 3 Feb 2023 12:43:37pm
	Author:  user

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

//JUCE_MODAL_LOOPS_PERMITTED 1

class AudioFileLoader {
public:
	AudioFileLoader();
	~AudioFileLoader();
	void openFile();
	void loadAudio(juce::File);

private:
	//juce::File* audioFile;
	juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource; // fichier d'origine
	juce::AudioBuffer<float>* buffer; // contient les samples audio (similaire à AudioSamplesBuffer)
	//juce::AudioThumbnail* thumbnail; // pour dessiner la waveform
};
