#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/GuiTypes.h"
#include "gui/panel/botrow/graintab/GrainsPanel.h"

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
        void showTab(int tabIndex);

        UIContext& uic;

        juce::TextButton grainTabButton{"GRAINS"};
        juce::TextButton modTabButton{"TEMP"};
        juce::TextButton regionTabButton{"TEMP"};

        GrainsPanel grainsPanel;
        juce::Component modTempPanel;
        juce::Component regionTempPanel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabPanel)
    };
}