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

#pragma once



class ThumbnailComponent : public juce::Component, private juce::ChangeListener
{
public:
	ThumbnailComponent(int, juce::AudioFormatManager&, juce::AudioThumbnailCache&);
	~ThumbnailComponent();

	void setFile(const juce::File& );

	void paint(juce::Graphics& ) override;
	void paintIfNoFileLoaded(juce::Graphics& );
	void paintIfFileLoaded(juce::Graphics& );

	void changeListenerCallback(juce::ChangeBroadcaster* ) override;

private:

	juce::AudioThumbnail thumbnail;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
};