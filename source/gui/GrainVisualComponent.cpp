/*
  ==============================================================================

	GrainVisualComponent.cpp
	Created: 12 Apr 2023 1:47:43pm
	Author:  user

  ==============================================================================
*/

#include "GrainVisualComponent.h"

GrainVisualComponent::GrainVisualComponent(GrainVisualBuffer& vb) : visualBuffer{vb}
{
    startTimerHz(30);
    colour = MyColours::lavender;
    numSamples = 0;
}

GrainVisualComponent::~GrainVisualComponent()
{
    stopTimer();
}

void GrainVisualComponent::paint(juce::Graphics& g)
{

    //for(Grain* grain : *grains)
    //{
    //    const float samplePos =
    //        static_cast<float>(grain->getGrainPoint()->samplePos) / static_cast<float>(numSamples * getWidth());
    //    const float yPos = static_cast<float>(grain->getGrainPoint()->yPos) * static_cast<float>(getHeight());
    //    const float opacity = grain->getGrainPoint()->opacity;

    //    g.setColour(colour.withAlpha(opacity));
    //    g.fillEllipse(samplePos, (float)yPos, SIZE, SIZE);
    //}
}

void GrainVisualComponent::setNumSamples(int numSamples) { this->numSamples = numSamples; }

void GrainVisualComponent::timerCallback() { repaint(); }

void GrainVisualComponent::resized() {}

//void GrainVisualComponent::setGrains(juce::Array<Grain*>* grains) { this->grains = grains; }