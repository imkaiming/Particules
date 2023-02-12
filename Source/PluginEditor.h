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

	void initButtons();

	// Core functions
	GranularSynthesisAudioProcessor& audioProcessor;
	StateSaver* stateSaver;
	AudioFileLoader loader;


	// Buttons parameters
	juce::DrawableButton open_btn;
	juce::DrawableButton stop_btn;
	juce::DrawableButton play_btn;

	// Buttons methods
	void openFileButtonClicked();
	void stopFileButtonClicked();
	void playFileButtonClicked();

	// GUI
	MainFrame mainFrame;
	CustomLookAndFeel customLookAndFeel;
	int width, heigth;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GranularSynthesisAudioProcessorEditor)


};
