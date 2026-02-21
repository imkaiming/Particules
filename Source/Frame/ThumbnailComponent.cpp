/*
  ==============================================================================

	ThumbnailComponent.cpp
	Created: 3 Feb 2023 10:19:39pm
	Author:  user

  ==============================================================================
*/

#include "ThumbnailComponent.h"

#include "../Framework/ParamsID.h"
#include "../Utils/MyColours.h"
#include "../Framework/ParameterView.h"
#include "../Framework/UIContext.h"


ThumbnailComponent::ThumbnailComponent(int samplesPerThumbnail, juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache, UIContext& uic)
	: thumbnail(samplesPerThumbnail, formatManager, cache), grainVisualizer(uic.paramsView.getGrains()), paramsView(uic.paramsView), apvts{uic.apvts}
{
	paramsView.setGrainVisualizer(&grainVisualizer);

	positionValue = Param::Position::init;
	selectionValue = Param::Selection::init;
	

	updatePosition(positionValue);
	updateSelection(selectionValue);
	updateOverflow(positionValue);

	thumbnail.addChangeListener(this);

	addAndMakeVisible(&selection);
	addAndMakeVisible(&position);
	addAndMakeVisible(&overflow);
	addAndMakeVisible(&grainVisualizer);

	// apvts listener to update the UI
	apvts.addParameterListener(Param::Position::id, this);
	apvts.addParameterListener(Param::Selection::id, this);
}

ThumbnailComponent::~ThumbnailComponent()
{
	thumbnail.removeChangeListener(this);
	apvts.removeParameterListener(Param::Position::id, this);
	apvts.removeParameterListener(Param::Selection::id, this);
	paramsView.setGrainVisualizer(nullptr);
}

void ThumbnailComponent::setFile(const juce::File& file)
{
	thumbnail.setSource(new juce::FileInputSource(file));
}

void ThumbnailComponent::paint(juce::Graphics& g)
{
	if(thumbnail.getNumChannels() == 0)
		paintIfNoFileLoaded(g);
	else
		paintIfFileLoaded(g);
}

void ThumbnailComponent::paintIfNoFileLoaded(juce::Graphics& g)
{
	g.fillAll(MyColours::black);
	g.setColour(MyColours::cream);
	g.drawFittedText("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
}

void ThumbnailComponent::paintIfFileLoaded(juce::Graphics& g)
{
	g.fillAll(MyColours::black);

	g.setColour(MyColours::brightBlue);
	thumbnail.drawChannels(g, getLocalBounds(), 0.0, thumbnail.getTotalLength(), 1.0f);
}

void ThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	if(source == &thumbnail)
	{
		repaint();
		if(thumbnail.isFullyLoaded())
		{
			//juce::Logger::outputDebugString("FINI DE PEINTURER CALLBACK");
			// TODO INSERT AUDIO FILE FRAME CALLBACK IF YOU WANT 
			// TO ENABLE PLAY BTN ONLY AFTER THE WAVEFORM IS SET 

			if(onThumbnailReady)
				onThumbnailReady();

		}
	}
}

void ThumbnailComponent::parameterChanged(const juce::String& parameterID, float newValue)
{
	if(parameterID == Param::Position::id)
	{
		//DBG("parameter view position new value : " + juce::String(paramsView.getFilePosition()));
		updatePosition(newValue);
		return;
	}
	if(parameterID == Param::Selection::id)
	{
		//DBG("parameter view selection new value : " + juce::String(paramsView.getWindowSelection()));
		updateSelection(newValue);
		return;
	}
	if(parameterID == Param::Mix::id)
	{
		DBG("parameter view MIX new value : " + juce::String(paramsView.getMix()));
		return;
	}
	if(parameterID == Param::Gain::id)
	{
		DBG("parameter view GAIN new value : " + juce::String(paramsView.getGain()));
		return;
	}
	if(parameterID == Param::Position::id)
	{
		//DBG("parameter view position new value : " + juce::String(paramsView.getFilePosition()));
		return;
	}
	if(parameterID == Param::Selection::id)
	{
		//DBG("parameter view selection new value : " + juce::String(paramsView.getWindowSelection()));
		return;
	}
}

void ThumbnailComponent::updatePosition(float value)
{
	positionValue = value;
	position.setPosition(positionValue * getWidth());
	selection.setPosition(positionValue * getWidth());
	//DBG("update pos is called");
	float overflow = selectionValue * getWidth() + positionValue * getWidth() - (float)getWidth();
	if(overflow >= 0.f)
		updateOverflow(overflow);
	else
		updateOverflow(0.f);
}

void ThumbnailComponent::updateSelection(float value)
{
	selectionValue = value;
	selection.setSelection(selectionValue * getWidth());

	float overflow = selectionValue * getWidth() + positionValue * getWidth() - (float)getWidth();
	if(overflow >= 0.f)
		updateOverflow(overflow);
	else
		updateOverflow(0.f);

}

void ThumbnailComponent::setCallbackOnThumbnailReady(std::function<void()> foo)
{
	onThumbnailReady = std::move(foo);
}

void ThumbnailComponent::updateOverflow(float value)
{
	overflow.setSelection(value);
}

void ThumbnailComponent::resized()
{
	updatePosition(positionValue);
	updateSelection(selectionValue);

	position.setBounds(getLocalBounds());
	selection.setBounds(getLocalBounds());
	overflow.setBounds(getLocalBounds());
	grainVisualizer.setBounds(getLocalBounds());
}

