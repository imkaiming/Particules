/*
  ==============================================================================

    GrainsFrame.h
    Created: 3 Feb 2023 11:44:06pm
    Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include "../Utils/StateParameters.h"
#include "../Utils/MyColours.h"

#pragma once

class GrainsFrame : public juce::Component, public juce::Slider::Listener
{
public:
	GrainsFrame(ValueTreeState* apvts, StateParameters* stateParams);
	~GrainsFrame() override;

	void paint(juce::Graphics&) override;
	void resized() override;

	void sliderValueChanged(juce::Slider* slider) override;

private:

	StateParameters* stateParams;
	ValueTreeState* apvts;

	juce::Slider densitySlider;
	juce::Slider durationSlider;
	juce::Slider speedSlider;
	juce::Slider envWidthSlider;

	juce::Label densityLabel;
	juce::Label durationLabel;
	juce::Label speedLabel;
	juce::Label envWidthLabel;


	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> densitySliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> durationSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> speedSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> envWidthSliderAttachment;

	juce::ComboBox envelopeList;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> envelopeAttachment;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsFrame)
};