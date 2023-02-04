/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"

AudioFileFrame::AudioFileFrame() {
	//openFileButton.setButtonText("Load File");
	//openFileButton.set
	 //addAndMakeVisible(&openFileButton);
	 addAndMakeVisible(spectrumComponent);
}
AudioFileFrame::~AudioFileFrame() {

}

void AudioFileFrame::paint(juce::Graphics& g) {
	juce::Rectangle<float> audioFileFrame(0.f, 0.f, getWidth(), getHeight());
	g.setColour(juce::Colours::slategrey);
	g.fillRect(audioFileFrame);
	g.drawRect(audioFileFrame);
}
void AudioFileFrame::resized() {
	//openFileButton.setBounds(25.f, 1.f, 125.f, 33.3f);
	spectrumComponent.setBounds(getLocalBounds());

}