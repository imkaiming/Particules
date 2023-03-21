/*
  ==============================================================================

	AudioFileFrame.h
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/ThumbnailComponent.h"
#include "../Utils/AudioFileLoader.h"
#include "../Utils/StateParameters.h"
#include "../Utils/MyColours.h"



class AudioFileFrame :
	public juce::Component, public juce::FileDragAndDropTarget, public juce::Value::Listener // public juce::DrawableButton::Listener,
{
public:
	AudioFileFrame(ValueTreeState* apvts, StateParameters* stateParams);
	~AudioFileFrame() override;
	void paint(juce::Graphics&) override;
	void resized() override;

	void valueChanged(juce::Value& value) override;

private:

	//std::unique_ptr<AudioFileLoader> loader; // load the audio file and pass it to the stateparameter
	AudioFileLoader loader;
	juce::AudioThumbnailCache thumbnailCache;
	ThumbnailComponent thumbnailComponent; // after the file is loaded draw the waveform

	juce::Value* isAudioLoaded;
	ValueTreeState* apvts;
	StateParameters* stateParams;

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


	// marche pas
	bool isInterestedInFileDrag(const juce::StringArray&);
	void filesDropped(const juce::StringArray&, int, int);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileFrame)

};
