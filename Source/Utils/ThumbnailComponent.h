/*
  ==============================================================================

	SpectrumComponent.h
	Created: 3 Feb 2023 10:19:39pm
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ParamsID.h"
#include "../Utils/MyColours.h"
#include "PositionOverlay.h"
#include "StateParameters.h"

#pragma once



class ThumbnailComponent : public juce::Component, private juce::ChangeListener //, public juce::Value::Listener
{
public:
	ThumbnailComponent(int, juce::AudioFormatManager&, juce::AudioThumbnailCache&);// , StateParameters*);
	~ThumbnailComponent();

	void setFile(const juce::File&);

	void paint(juce::Graphics&) override;
	void resized() override;
	void paintIfNoFileLoaded(juce::Graphics&);
	void paintIfFileLoaded(juce::Graphics&);

	void changeListenerCallback(juce::ChangeBroadcaster*) override; // to change the waveform when changing the audio

	void updateFilePostion(float value); // update the position marker when changing the file position slider value


private:

	float positionValue;

	//StateParameters* stateParams;
	juce::AudioThumbnail thumbnail;
	PositionOverlay positionMarker;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
};