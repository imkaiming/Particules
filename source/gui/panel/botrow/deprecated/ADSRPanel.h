#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
//#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/GuiTypes.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    class ADSRPanel : public juce::Component
    {
    public:
        ADSRPanel(UIContext& uic);
        ~ADSRPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        UIContext& uic;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRPanel)
    };
}