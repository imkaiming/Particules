#pragma once

#include "../../framework/Constants.h"
#include "../../framework/Core.h"
#include "../../utils/PluginParams.h"
#include "../../utils/struct/UIContext.h"

#include "ADSRPanel.h"
#include "GrainsPanel.h"
#include "SynthPanel.h"

namespace particules
{

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