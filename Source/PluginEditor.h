/**
 * This class is the graphical user interface for the plugin and is responsible
 * for displaying the controls and parameters.
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Frame/MainFrame.h"
#include "Utils/CustomLookAndFeel.h"


class ParticulesAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	ParticulesAudioProcessorEditor(ParticulesAudioProcessor&);
	~ParticulesAudioProcessorEditor() override;
	void paint(juce::Graphics&) override;
	void resized() override;

private:

	void initButtons();

	// Core functions
	ParticulesAudioProcessor& audioProcessor;
	StateSaver* stateSaver;
	std::unique_ptr<AudioFileLoader> loader;

	//juce::Slider gainSlider;
	//gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalDrag);
	//gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);


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


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessorEditor)


};
