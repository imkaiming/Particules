
#pragma once

#include "../../framework/Core.h"
#include "../../framework/bridge/EngineState.h"
#include "../../utils/struct/UIContext.h"


namespace particules
{

    //struct UIContext;
    class ParticulesAudioProcessor;
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