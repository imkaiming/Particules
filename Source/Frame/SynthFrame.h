/*
  ==============================================================================

	SynthFrame.h
	Created: 3 Feb 2023 11:44:16pm
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include "../Utils/StateParameters.h"
#include "../Utils/MyColours.h"
#include "../Utils//ThumbnailComponent.h"
#pragma once

class SynthFrame : public juce::Component//, public juce::Slider::Listener
{
public:
	SynthFrame(ValueTreeState* apvts, StateParameters* stateParams);
	~SynthFrame();

	void paint(juce::Graphics&) override;
	void resized() override;

	//void sliderValueChanged(juce::Slider* slider) override;

	void init(ThumbnailComponent*);

private:

	ValueTreeState* apvts;
	StateParameters* stateParams;
	ThumbnailComponent* thumbnailComponent;

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