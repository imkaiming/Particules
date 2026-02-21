/*
  ==============================================================================

	SynthFrame.h
	Created: 3 Feb 2023 11:44:16pm
	Author:  user

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "../Frame/ThumbnailComponent.h"

// the Synth Frame provide UI control that modify the apvts values only

class SynthFrame : public juce::Component
{
public:
	SynthFrame(ValueTreeState& apvts);
	~SynthFrame() = default;

	void paint(juce::Graphics&) override;
	void resized() override;

	//void sliderValueChanged(juce::Slider* slider) override;

	//void init(ThumbnailComponent&);

private:

	//ThumbnailComponent& thumbnailComponent;
	//ValueTreeState& apvts;

	juce::Slider mixSlider;
	juce::Slider gainSlider;

	juce::Label mixLabel;
	juce::Label gainLabel;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;


	juce::Slider filePosSlider;
	juce::Slider windowSelectionSlider;

	juce::Label filePosLabel;
	juce::Label windowSelectionLabel;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filePosSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> windowSelectionSliderAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthFrame)

};