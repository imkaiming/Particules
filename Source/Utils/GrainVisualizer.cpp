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
}


void GrainVisualizer::paint(juce::Graphics& g)
{
	// on évite le premier paint qui est appeler lors de la construction.
	if (grains == nullptr)
	{
		return;
	}

	Grain* grain = grains->getFirst();
	/*for (Grain* const& grain : *grains)
	{*/
	int samplePos = grain->getGrainPoint()->getSamplePos() / numSamples * getWidth();
	int yPos = grain->getGrainPoint()->getYpos() * getHeight();

	g.setColour(colour.withAlpha(grain->getGrainPoint()->getOpacity()));
	g.fillEllipse(samplePos, yPos, GRAINPOINT_SIZE, GRAINPOINT_SIZE);
	//}
}

//void GrainVisualizer::update()
//{
//	repaint();
//}


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