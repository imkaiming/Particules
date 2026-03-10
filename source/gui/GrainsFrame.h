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
    juce::Slider emissionSlider;
	juce::Slider durationSlider;
	juce::Slider speedSlider;
    juce::Slider sustainRatioSlider;
	juce::Slider traversalFreqSlider;

	juce::Label emissionLabel;
    juce::Label durationLabel;
    juce::Label speedLabel;
    juce::Label sustainRatioLabel;
    juce::Label envModeLabel;
    juce::Label traversalModeLabel;
    juce::Label traversalFreqLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> emissionSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> durationSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> speedSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainRatioSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> traversalFreqSliderAttachment;


	juce::ComboBox envelopeModeList;
	juce::ComboBox traversalModeList;

	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> envelopeModeAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> traversalModeAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsFrame)
};