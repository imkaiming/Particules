#pragma once

//#include "../../framework/Constants.h"
//#include "../../utils/PluginParams.h"

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "ADSRPanel.h"
#include "GrainsPanel.h"
#include "SynthPanel.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    class ControlPanel : public juce::Component
    {
    public:
        ControlPanel(UIContext& uic);
        ~ControlPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        UIContext& uic;

        GrainsPanel grainsPanel;
        SynthPanel synthPanel;
        ADSRPanel adsrPanel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlPanel)
    };
}