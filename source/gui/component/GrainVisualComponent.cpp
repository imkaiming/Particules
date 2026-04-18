#include "GrainVisualComponent.h"

#include "framework/state/AudioState.h"
#include "framework/state/UIState.h"
#include "gui/lookandfeelv2/Colours.h"
#include "utils/struct/UIContext.h"
#include "utils/struct/VisualSnapshot.h"

namespace particules
{

    GrainVisualComponent::GrainVisualComponent(UIContext& uic)
        : uiState{uic.uiState}, numSamples{uic.audioState.getNumSamples()}, invWidthSamples{0.f}, colour{colours::lavender}
    {
        setOpaque(false);
        //startTimerHz(static_cast<int>(gui::refreshRate));
    }

    //GrainVisualComponent::~GrainVisualComponent() { stopTimer(); }

    void GrainVisualComponent::paint(juce::Graphics& g)
    {
        if(invWidthSamples == 0.f)
            return;
        const VisualSnapshot& snap = uiState.getSnapshot();

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

    void GrainVisualComponent::setNumSamples(int val)
    {
        if(val > 0)
        {
            numSamples = val;
            resized();
        }
        else
            invWidthSamples = 0.f;
    }

    //void GrainVisualComponent::timerCallback() { repaint(); }

    void GrainVisualComponent::resized()
    {
        if(numSamples > 0)
            invWidthSamples = static_cast<float>(getWidth()) / static_cast<float>(numSamples);
    }
}

//void GrainVisualComponent::setGrains(juce::Array<Grain*>* grains) { this->grains = grains; }