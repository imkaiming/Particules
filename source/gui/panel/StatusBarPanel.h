
#pragma once

#include "../../framework/Core.h"
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
        ParticulesAudioProcessor& audioProcessor;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarPanel)
    };
}