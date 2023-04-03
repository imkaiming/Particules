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
	juce::Slider traversalTimeSlider;

	juce::Label densityLabel;
	juce::Label durationLabel;
	juce::Label speedLabel;
	juce::Label envWidthLabel;
	juce::Label envTypeLabel;
	juce::Label traversalModeLabel;
	juce::Label traversalTimeLabel;


	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> densitySliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> durationSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> speedSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> envWidthSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> traversalTimeSliderAttachment;


	juce::ComboBox envelopeList;
	juce::ComboBox traversalModeList;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> envelopeAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> traversalModeAttachment;




	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsFrame)
};