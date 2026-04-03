#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../framework/Core.h"
#include "../../framework/bridge/EngineState.h"

namespace particules
{
    class EngineState;
    struct UIContext;
    class StatusBarPanel : public juce::Component, public juce::Timer
    {
    public:
        StatusBarPanel(UIContext& uic);
        ~StatusBarPanel() = default;
        void paint(juce::Graphics&) override;
        void resized() override;
        void timerCallback() override;

    private:
        juce::Label statusLabel;
        UIContext& uic;
        EngineState& engineState;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarPanel)
    };
}