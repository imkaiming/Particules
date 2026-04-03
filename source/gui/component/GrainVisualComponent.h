#pragma once

#include <juce_core/juce_core.h> // end macro
#include <juce_events/juce_events.h> // juce::Timer
#include <juce_graphics/juce_graphics.h> // juce::Colour
#include <juce_gui_basics/juce_gui_basics.h> // juce::Component

#include "../../framework/Constants.h"

/**
* On a besoin de la valeur de la position du grain dans le buffer pour montrer sa position sur l'axe x.
* et on a besoin de la valeur de son relative à enveloppe pour la peindre sa transparence.
* Plus un son est fort plus il est opaque.
* Sur l'axe Y, les valeurs seront disposés aléatoirement.
*
* La classe GrainVisualComponent est appelé avec update à chaque lecture de sample ou à chaque chunk de buffer
*/

namespace juce
{
    class Graphics;
}

namespace particules
{

    struct UIContext;
    class UIState;
    class GrainVisualComponent : public juce::Component, private juce::Timer
    {
    public:
        GrainVisualComponent(UIContext&);
        ~GrainVisualComponent() override;

        void paint(juce::Graphics& g) override;
        void resized() override;

        void setNumSamples(int);

    private:
        static constexpr int MAXGRAINS = static_cast<int>(global::maxGrains);
        static constexpr float GSIZE = UI::grainVisualSize;
        static constexpr float GCENTER = GSIZE / 2.f;
        void timerCallback() override;

        juce::Colour colour;
        UIState& uiState;
        int numSamples;

        float invWidthSamples;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainVisualComponent)
    };
}