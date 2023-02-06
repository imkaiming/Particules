/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"

AudioFileFrame::AudioFileFrame(AudioFileComponent* audioFileComponent)
{
	//std::unique_ptr <juce::XmlElement> addFolder_Fill_svg(juce::XmlDocument::parse(BinaryData::AddFolder_Fill_svg));
	//std::unique_ptr <juce::XmlElement> addFolder_svg(juce::XmlDocument::parse(BinaryData::AddFolder_svg));
	this->audioFileComponent = audioFileComponent;
	addAndMakeVisible(*audioFileComponent);
	addAndMakeVisible(&spectrumComponent);
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
	float w = getWidth() / 30.f;
	float h = getHeight() / 30.f;
	//int bordure = getWidth() 

	area.removeFromTop(static_cast<int>(h));
	area.removeFromBottom(static_cast<int>(h));
	audioFileComponent->setBounds(area.removeFromLeft(static_cast<int>(w)));
	area.removeFromLeft(static_cast<int>(h));
	area.removeFromRight(static_cast<int>(h));
	spectrumComponent.setBounds(area.removeFromLeft(area.getWidth()));

}

void AudioFileFrame::init(StateSaver* stateSaver) {
	audioFileComponent->init(stateSaver);
}