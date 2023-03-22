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
	updateFilePosition(positionValue);
	updateSelection(selectionValue);

	thumbnail.addChangeListener(this);

	addAndMakeVisible(&positionComponent);
	addAndMakeVisible(&selectionComponent);
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

void ThumbnailComponent::setAvailableSpace()
{
	availableSpace = getWidth() - positionValue;
}

void ThumbnailComponent::updateFilePosition(float value)
{
	positionValue = value * getWidth();
	setAvailableSpace();

	positionComponent.setPosition(positionValue);
	selectionComponent.setPosition(positionValue);

	updateSelection(selectionValue);

}

void ThumbnailComponent::updateSelection(float value)
{
	selectionValue = value ;
	selectionComponent.setSelection(selectionValue * availableSpace);
}

void ThumbnailComponent::resized()
{
	positionComponent.setPosition(positionValue);
	positionComponent.setBounds(getLocalBounds());

	selectionComponent.setPosition(positionValue);
	selectionComponent.setSelection(selectionValue);
	selectionComponent.setBounds(getLocalBounds());


}
