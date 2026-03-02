/*
  ==============================================================================

	GrainsFrame.h
	Created: 3 Feb 2023 11:44:06pm
	Author:  user

  ==============================================================================
*/
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

#include "../framework/Core.h"

class GrainsFrame: public juce::Component
{
public:
	GrainsFrame(ValueTreeState& apvts);
	~GrainsFrame() = default;

	void paint(juce::Graphics&) override;
	void resized() override;

private:
	juce::Slider densitySlider;
	juce::Slider durationSlider;
	juce::Slider speedSlider;
    juce::Slider sustainRatioSlider;
	juce::Slider traversalFreqSlider;

	juce::Label densityLabel;
	juce::Label durationLabel;
	juce::Label speedLabel;
    juce::Label sustainRatioLabel;
	juce::Label envTypeLabel;
	juce::Label traversalModeLabel;
	juce::Label traversalFreqLabel;


	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> densitySliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> durationSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> speedSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainRatioSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> traversalFreqSliderAttachment;


	juce::ComboBox envelopeList;
	juce::ComboBox traversalModeList;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> envelopeAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> traversalModeAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsFrame)
};