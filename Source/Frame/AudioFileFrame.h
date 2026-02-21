/*
  ==============================================================================

	AudioFileFrame.h
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#pragma once


#include "ThumbnailComponent.h"
#include "../Framework/AudioFileLoader.h"

// the Audio File Frame provide the control to load and play the audio.

//class ParameterView;
//class SynthFrame;
struct UIContext;
class AudioFileFrame:	public juce::Component, public juce::FileDragAndDropTarget
{
public:
	//AudioFileFrame(ParameterView&, SynthFrame&);
	AudioFileFrame(UIContext& uic);
	~AudioFileFrame() = default;
	void paint(juce::Graphics&) override;
	void resized() override;

private:

	ParameterView& paramsView;
	AudioFileLoader loader;
	juce::AudioThumbnailCache thumbnailCache;
	ThumbnailComponent thumbnailComponent; // after the file is loaded draw the waveform


	// Buttons parameters
	juce::DrawableButton open_btn;
	juce::DrawableButton stop_btn;
	juce::DrawableButton play_btn;

	// Button managment
	void setPlayButtonImagePlay();
	void setPlayButtonImagePause();
	void setOpenButtonImageOpen();
	void setStopButtonImageStop();

	// Buttons methods
	//void initDrawableButtons();
	void openFileButtonClicked();
	void stopFileButtonClicked();
	void playFileButtonClicked();

	bool isInterestedInFileDrag(const juce::StringArray&);
	void filesDropped(const juce::StringArray&, int, int);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileFrame)

};
