/*
  ==============================================================================

	ControlFrame.h
	Created: 2 Mar 2023 7:45:17pm
	Author:  user

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../Utils/ParamsID.h"
#include "../Utils/StateParameters.h"

class ControlFrame : public juce::Component, public juce::Slider::Listener
{

public:
	ControlFrame(juce::AudioProcessorValueTreeState*, StateParameters*); // , StateParameters* stateParams
	~ControlFrame();
	void paint(juce::Graphics&) override;
	void resized() override;
	void sliderValueChanged(juce::Slider* slider) override;

private:

	ValueTreeState* apvts;
	StateParameters* stateParams;

	juce::Slider mixSlider;
	juce::Slider gainSlider;
	juce::Slider densitySlider;
	juce::Slider durationSlider;
	juce::Slider speedSlider;

	juce::Label mixLabel;
	juce::Label gainLabel;
	juce::Label densityLabel;
	juce::Label durationLabel;
	juce::Label speedLabel;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> densitySliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> durationSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> speedSliderAttachment;

	juce::ComboBox envelopeList;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> envelopeAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlFrame)
};