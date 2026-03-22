
#pragma once

#include "../framework/Core.h"

namespace particules
{

    class CustomLookAndFeel;
    class TitleFrame : public juce::Component
    {
    public:
        TitleFrame(CustomLookAndFeel& look);
        ~TitleFrame() = default;
        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        juce::Label titreLabel;
        CustomLookAndFeel& look;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleFrame)
    };
}