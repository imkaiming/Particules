/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"

AudioFileFrame::AudioFileFrame(juce::DrawableButton* open_Btn, juce::DrawableButton* play_Btn,
	juce::DrawableButton* stop_Btn) :
	audioFileComponent(open_Btn, play_Btn, stop_Btn)
{
	addAndMakeVisible(&audioFileComponent);
	addAndMakeVisible(&spectrumComponent);
}

AudioFileFrame::~AudioFileFrame() {

}

void AudioFileFrame::paint(juce::Graphics& g) {
	g.fillAll(juce::Colours::coral);
}
void AudioFileFrame::resized() {
	juce::Rectangle<int> area = getLocalBounds();
	float w = getWidth() / 30.f;
	float h = getHeight() / 30.f;

	area.removeFromTop(static_cast<int>(h));
	area.removeFromBottom(static_cast<int>(h));
	audioFileComponent.setBounds(area.removeFromLeft(static_cast<int>(w)));
	area.removeFromLeft(static_cast<int>(h));
	area.removeFromRight(static_cast<int>(h));
	spectrumComponent.setBounds(area.removeFromLeft(area.getWidth()));

}