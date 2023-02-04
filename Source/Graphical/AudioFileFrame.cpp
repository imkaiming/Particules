/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"

AudioFileFrame::AudioFileFrame() {
	addAndMakeVisible(spectrumComponent);
}
AudioFileFrame::~AudioFileFrame() {

}

void AudioFileFrame::paint(juce::Graphics& g) {
	juce::Rectangle<float> audioFileFrame(25.f, 1.f, getWidth()-50.f, 100.f);
	
	g.setColour(juce::Colours::coral);
	g.fillRect(audioFileFrame);
	g.drawRect(audioFileFrame);
}
void AudioFileFrame::resized() {
	spectrumComponent.setBounds(getLocalBounds());

}