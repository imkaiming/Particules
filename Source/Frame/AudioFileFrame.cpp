/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"

AudioFileFrame::AudioFileFrame()
{
	//std::unique_ptr <juce::XmlElement> addFolder_Fill_svg(juce::XmlDocument::parse(BinaryData::AddFolder_Fill_svg));
	//std::unique_ptr <juce::XmlElement> addFolder_svg(juce::XmlDocument::parse(BinaryData::AddFolder_svg));

	addAndMakeVisible(audioFileComponent);
	addAndMakeVisible(spectrumComponent);
}

AudioFileFrame::~AudioFileFrame() {

}

void AudioFileFrame::paint(juce::Graphics& g) {
	//juce::Rectangle<float> audioFileFrame(0.f, 0.f, getWidth(), getHeight());
	//g.setColour(juce::Colours::slategrey);
	//g.fillRect(audioFileFrame);
	//g.drawRect(audioFileFrame);
	g.fillAll(juce::Colours::coral);
}
void AudioFileFrame::resized() {
	juce::Rectangle<int> area = getLocalBounds();
	int w = getWidth() / 30;
	int h = getHeight() / 30;
	//int bordure = getWidth() 

	area.removeFromTop(h);
	area.removeFromBottom(h);
	audioFileComponent.setBounds(area.removeFromLeft(w));
	spectrumComponent.setBounds(area.removeFromLeft(w * 29));

}