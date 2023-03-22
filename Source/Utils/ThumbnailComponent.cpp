/*
  ==============================================================================

	ThumbnailComponent.cpp
	Created: 3 Feb 2023 10:19:39pm
	Author:  user

  ==============================================================================
*/

#include "ThumbnailComponent.h", 

ThumbnailComponent::ThumbnailComponent(int samplesPerThumbnail,
	juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& cache)
	: thumbnail(samplesPerThumbnail, formatManager, cache)
{
	positionValue = 0.f;
	selectionValue = 1.f;
	updatePosition(positionValue);
	updateSelection(selectionValue);

	thumbnail.addChangeListener(this);

	addAndMakeVisible(&selectionComponent);
	addAndMakeVisible(&positionComponent);

}

ThumbnailComponent::~ThumbnailComponent()
{

}

void ThumbnailComponent::setFile(const juce::File& file)
{
	thumbnail.setSource(new juce::FileInputSource(file));
}

void ThumbnailComponent::paint(juce::Graphics& g)
{
	if (thumbnail.getNumChannels() == 0)
		paintIfNoFileLoaded(g);
	else
		paintIfFileLoaded(g);
}

void ThumbnailComponent::paintIfNoFileLoaded(juce::Graphics& g)
{
	g.fillAll(MyColours::black);
	g.setColour(MyColours::cream);
	g.drawFittedText("No File Loaded",
		getLocalBounds(),
		juce::Justification::centred,
		1);
}

void ThumbnailComponent::paintIfFileLoaded(juce::Graphics& g)
{
	g.fillAll(MyColours::black);

	g.setColour(MyColours::brightBlue);
	thumbnail.drawChannels(g,
		getLocalBounds(),
		0.0,
		thumbnail.getTotalLength(),
		1.0f);
}

void ThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	if (source == &thumbnail)
		repaint();
}

void ThumbnailComponent::updatePosition(float value)
{
	positionValue = value;
	positionComponent.setPosition(positionValue * getWidth());
	selectionComponent.setPosition(positionValue * getWidth());
	//updateSelection(selectionValue);

}

void ThumbnailComponent::updateSelection(float value)
{
	selectionValue = value;
	//float space = getWidth() - (positionValue * getWidth());
	selectionComponent.setSelection(selectionValue * getWidth());

}

void ThumbnailComponent::resized()
{
	updatePosition(positionValue);
	updateSelection(selectionValue);
	positionComponent.setBounds(getLocalBounds());
	selectionComponent.setBounds(getLocalBounds());
}
