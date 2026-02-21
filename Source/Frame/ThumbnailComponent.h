/*
  ==============================================================================

	SpectrumComponent.h
	Created: 3 Feb 2023 10:19:39pm
	Author:  user

  ==============================================================================
*/
#pragma once

//#include <JuceHeader.h>


#include "PositionOverlay.h"
#include "SelectionOverlay.h"

#include "../Utils/GrainVisualizer.h"
#include "../Framework/Core.h"

// Thumbnail is where the audio waveform is painted after being loaded by the audio file loader

class ParameterView;
struct UIContext;
class ThumbnailComponent: public juce::Component, private juce::ChangeListener, juce::AudioProcessorValueTreeState::Listener
{
public:
	ThumbnailComponent(int, juce::AudioFormatManager&, juce::AudioThumbnailCache&, UIContext&);
	~ThumbnailComponent() override;

	void setFile(const juce::File&);


	void setCallbackOnThumbnailReady(std::function<void()>);

	GrainVisualizer* getGrainVisualizer() { return &grainVisualizer; };

private:

	void paint(juce::Graphics&) override;
	void resized() override;

	void paintIfNoFileLoaded(juce::Graphics&);
	void paintIfFileLoaded(juce::Graphics&);

	void changeListenerCallback(juce::ChangeBroadcaster*) override; // to change the waveform when changing the audio
	void parameterChanged(const juce::String& parameterID, float newValue) override;
	void updateOverflow(float value);
	void updatePosition(float value); // update the position marker when changing the file position slider value
	void updateSelection(float value);

	ParameterView& paramsView;
	ValueTreeState& apvts;

	float positionValue;
	float selectionValue;

	juce::AudioThumbnail thumbnail;
	std::function<void()> onThumbnailReady;

	PositionOverlay position;
	SelectionOverlay selection;
	SelectionOverlay overflow; // when the selection is higher than the width it wrapped back. it add flexibility

	GrainVisualizer grainVisualizer;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
};