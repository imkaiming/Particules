/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"

AudioFileFrame::AudioFileFrame() :
	openFileButton((const juce::String) "openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted) 

{
	//std::unique_ptr <juce::XmlElement> addFolder_Fill_svg(juce::XmlDocument::parse(BinaryData::AddFolder_Fill_svg));
	//std::unique_ptr <juce::XmlElement> addFolder_svg(juce::XmlDocument::parse(BinaryData::AddFolder_svg));

	openFileButton.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_Fill_svg, BinaryData::AddFolder_Fill_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_svg, BinaryData::AddFolder_svgSize).get(),
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	);

	addAndMakeVisible(openFileButton);
	openFileButton.setBounds(getLocalBounds());


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