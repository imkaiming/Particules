#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Graphical/MainFrame.h"


class GranularSynthesisAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	GranularSynthesisAudioProcessorEditor(GranularSynthesisAudioProcessor&);
	~GranularSynthesisAudioProcessorEditor() override;

	void paint(juce::Graphics&) override;
	void resized() override;

private:

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	GranularSynthesisAudioProcessor& audioProcessor;
	MainFrame mainFrame;



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GranularSynthesisAudioProcessorEditor)


};
