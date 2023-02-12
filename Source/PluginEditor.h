/**
 * This class is the graphical user interface for the plugin and is responsible 
 * for displaying the controls and parameters. 
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Frame/MainFrame.h"
#include "Utils/CustomLookAndFeel.h"


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
	CustomLookAndFeel customLookAndFeel;
	int width, heigth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GranularSynthesisAudioProcessorEditor)


};
