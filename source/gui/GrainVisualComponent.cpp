/*
  ==============================================================================

	GrainVisualComponent.cpp
	Created: 12 Apr 2023 1:47:43pm
	Author:  user

  ==============================================================================
*/

#include "GrainVisualComponent.h"

GrainVisualComponent::GrainVisualComponent(GrainVisualBuffer& vb)
    : visualBuffer{vb}, numSamples{0}, invWidthSamples{0.f}, colour{MyColours::lavender}
{
    startTimerHz(60);
}

GrainVisualComponent::~GrainVisualComponent() { stopTimer(); }

void GrainVisualComponent::paint(juce::Graphics& g)
{
    if(invWidthSamples == 0.f)
        return;
    const auto& snap = visualBuffer.getSnapshot();

    for(int i = 0; i < snap.count; ++i)
    {
        const GrainVisual& gv = snap.grainVisuals[i];
        const float samplePos = gv.xPos * invWidthSamples;
        const float yPos = gv.yPos * static_cast<float>(getHeight());
        const float opacity = gv.opacity;
        g.setColour(colour.withAlpha(opacity));
        g.fillEllipse(samplePos - GCENTER, yPos, GSIZE, GSIZE);
    }
}

void GrainVisualComponent::setNumSamples(const float val)
{
    if(val > 0)
    {
        numSamples = val;
        resized();
    }
    else
        invWidthSamples = 0.f;
}

void GrainVisualComponent::timerCallback() { repaint(); }

void GrainVisualComponent::resized()
{
    if(numSamples > 0)
        invWidthSamples = static_cast<float>(getWidth()) / numSamples;
}

//void GrainVisualComponent::setGrains(juce::Array<Grain*>* grains) { this->grains = grains; }