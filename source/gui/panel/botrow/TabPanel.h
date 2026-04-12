#pragma once

#include "../../../framework/GuiTypes.h"

#include "graintab/GrainsPanel.h"

// TODO include modtab and region tag later

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    class TabPanel : public juce::Component
    {
    public:
        TabPanel(UIContext& uic);
        ~TabPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        UIContext& uic;

        juce::TextButton grainTabButton, modTabButton, regionTabButton;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabPanel)
    };
}