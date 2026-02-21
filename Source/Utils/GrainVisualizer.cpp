/*
  ==============================================================================

	GrainVisualizer.cpp
	Created: 12 Apr 2023 1:47:43pm
	Author:  user

  ==============================================================================
*/

#include "GrainVisualizer.h"

GrainVisualizer::GrainVisualizer(juce::Array<Grain*>* grains) : grains(grains)
{
	startTimerHz(30);
	colour = MyColours::lavender;
	this->grains = nullptr;
	numSamples = 0;
}

GrainVisualizer::~GrainVisualizer()
{
	this->grains = nullptr;
	stopTimer();
}


void GrainVisualizer::paint(juce::Graphics& g)
{

	// on 	te le premier paint qui est appeler lors de la construction.
	if (grains == nullptr)
	{
		return;
	}


	//Grain* grain = grains->getFirst();
	for (Grain* grain : *grains)
	{
		int samplePos = static_cast<int>(grain->getGrainPoint()->getSamplePos() / numSamples * getWidth());
		int yPos = static_cast<int>(grain->getGrainPoint()->getYpos() * getHeight());
		float opacity = grain->getGrainPoint()->getOpacity();

		g.setColour(colour.withAlpha(opacity));
		g.fillEllipse((float)samplePos, (float)yPos, GRAINPOINT_SIZE, GRAINPOINT_SIZE);
	}
}


void GrainVisualizer::setNumSamples(int numSamples)
{
	this->numSamples = numSamples;
}


void GrainVisualizer::timerCallback()
{
	repaint();
}

void GrainVisualizer::resized()
{

}

void GrainVisualizer::setGrains(juce::Array<Grain*>* grains)
{
	this->grains = grains;
}