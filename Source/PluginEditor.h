/**
 * This class is the graphical user interface for the plugin and is responsible
 * for displaying the controls and parameters.
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Frame/MainFrame.h"
#include "Utils/CustomLookAndFeel.h"
#include "Utils/StateParameters.h"
#include "Utils/MyColours.h"


class ParticulesAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	ParticulesAudioProcessorEditor(ParticulesAudioProcessor&);
	~ParticulesAudioProcessorEditor() override;
	void paint(juce::Graphics&) override;
	void resized() override;

private:

	ParticulesAudioProcessor& audioProcessor;
	MainFrame mainFrame;
	CustomLookAndFeel customLookAndFeel;
	int width, heigth;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessorEditor)


};
