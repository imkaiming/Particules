/*
  ==============================================================================

	SynthFrame.h
	Created: 3 Feb 2023 11:44:16pm
	Author:  user

  ==============================================================================
*/


#pragma once

#include "../framework/Types.h"

#include "../gui/ThumbnailComponent.h"

// the Synth Frame provide UI control that modify the apvts values only

class SynthFrame : public juce::Component
{
public:
	SynthFrame(ValueTreeState& apvts);
	~SynthFrame() = default;

	void paint(juce::Graphics&) override;
	void resized() override;

private:

	juce::Slider mixSlider;
	juce::Slider gainSlider;
	juce::Slider positionSlider;
	juce::Slider selectionSlider;

	juce::Label mixLabel;
	juce::Label gainLabel;
    juce::Label positionLabel;
	juce::Label selectionLabel;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> positionSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> selectionSliderAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthFrame)

};