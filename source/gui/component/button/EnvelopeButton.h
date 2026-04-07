#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../../framework/audio/LookUpTable.h"

namespace particules
{

    class EnvelopeButton : juce::Component
    {
    public:
        EnvelopeButton();
        ~EnvelopeButton();

    private:

        juce::Image createEnvelopeIcon(const particules::LookUpTable& lut, int width, int height)
        {
            juce::Image img(juce::Image::ARGB, width, height, true);
            juce::Graphics g(img);

            juce::Path p;
            // ... (Boucle for pour lire la LUT et créer le Path, comme pour le paint())

            g.setColour(juce::Colours::white);
            g.strokePath(p, juce::PathStrokeType(1.5f));

            return img;
        }

        juce::Image hannEnv, linEnv, sqrtEnv, gaussEnv, expEnv;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeButton)
    };
}