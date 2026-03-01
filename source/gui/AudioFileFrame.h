/*
  ==============================================================================

	AudioFileFrame.h
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#pragma once
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "BinaryData.h"

#include "ThumbnailComponent.h"

// the Audio File Frame provide the control to load and play the audio.

class ParameterView;
class ParticulesAudioProcessor;
//class SynthFrame;
//struct UIContext;
class AudioFileFrame:	public juce::Component, public juce::FileDragAndDropTarget, public juce::ChangeListener
{
public:

	AudioFileFrame(UIContext& uic);
	~AudioFileFrame();
	void paint(juce::Graphics&) override;
	void resized() override;

private:

	void changeListenerCallback(juce::ChangeBroadcaster*) override;

	ParameterView& paramsView;
	ParticulesAudioProcessor& audioProcessor;
	ThumbnailComponent thumbnailComponent; // after the file is loaded draw the waveform


	// Buttons parameters
	juce::DrawableButton open_btn;
	juce::DrawableButton stop_btn;
    juce::DrawableButton play_pause_btn;

	// Button managment
	void setPlayButtonImage();
	void setPauseButtonImage();
	void setOpenButtonImage();
	void setStopButtonImage();

	// Buttons methods
	//void initDrawableButtons();
	void openFileButtonClicked();
	void stopAudioButtonClicked();
	void playAudioButtonClicked();

	bool isInterestedInFileDrag(const juce::StringArray&);
	void filesDropped(const juce::StringArray&, int, int);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileFrame)

};
