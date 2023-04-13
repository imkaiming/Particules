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
#include "SelectionOverlay.h"
#include "GrainVisualizer.h"

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

	void updatePosition(float value); // update the position marker when changing the file position slider value
	void updateSelection(float value);

	GrainVisualizer* getGrainVisualizer();

private:
	void updateOverflow(float value);


	float positionValue;
	float selectionValue;

	juce::AudioThumbnail thumbnail;
	PositionOverlay positionComponent;
	SelectionOverlay selectionComponent;
	SelectionOverlay overflowComponent; // its when the selection is higher than the width

	GrainVisualizer grainVisualizer;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
};