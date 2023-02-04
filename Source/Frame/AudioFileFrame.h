/*
  ==============================================================================

	AudioFileFrame.h
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Component/SpectrumComponent.h"
//#include "../Component/SVGButton.h"


class AudioFileComponent : public juce::Component {
public:
	AudioFileComponent() :
		openFileButton((const juce::String)"openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
		saveFileButton((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
		playFileButton((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted)
	{
		openFileButton.setImages(
			juce::Drawable::createFromImageData(
				BinaryData::AddFolder_svg, BinaryData::AddFolder_svgSize).get(),
			juce::Drawable::createFromImageData(
				BinaryData::AddFolder_Fill_svg, BinaryData::AddFolder_Fill_svgSize).get(),
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
		);

		saveFileButton.setImages(
			juce::Drawable::createFromImageData(
				BinaryData::SavePresets_svg, BinaryData::SavePresets_svgSize).get(),
			juce::Drawable::createFromImageData(
				BinaryData::SavePresets_Fill_svg, BinaryData::SavePresets_Fill_svgSize).get(),
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
		);

		playFileButton.setImages(
			juce::Drawable::createFromImageData(
				BinaryData::Play_svg, BinaryData::Play_svgSize).get(),
			juce::Drawable::createFromImageData(
				BinaryData::Play_Fill_svg, BinaryData::Play_Fill_svgSize).get(),
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
		);

		addAndMakeVisible(openFileButton);
		addAndMakeVisible(saveFileButton);
		addAndMakeVisible(playFileButton);
	}

	~AudioFileComponent() override {

	}

	void paint(juce::Graphics&) override {

	}
	void resized() override {
		juce::Rectangle<int> area = getLocalBounds();
		int h = getHeight() / 3;

		openFileButton.setBounds(area.removeFromTop(h));
		saveFileButton.setBounds(area.removeFromTop(h));
		playFileButton.setBounds(area.removeFromTop(h));

	}

private:
	juce::DrawableButton openFileButton;
	juce::DrawableButton saveFileButton;
	juce::DrawableButton playFileButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileComponent)
};

class AudioFileFrame : public juce::Component {
public:
	AudioFileFrame();
	~AudioFileFrame() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:

	AudioFileComponent audioFileComponent;
	SpectrumComponent spectrumComponent;
	//juce::TextButton openFileButton;
	//juce::ImageButton openFileButton;
	//std::unique_ptr<juce::Drawable> openFileSvgPtr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileFrame)

};
