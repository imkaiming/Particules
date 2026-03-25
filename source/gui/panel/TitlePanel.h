
#pragma once

#include "../../framework/Core.h"

namespace particules
{

    class CustomLookAndFeel;
    class TitlePanel : public juce::Component
    {
    public:
        TitlePanel(CustomLookAndFeel& look);
        ~TitlePanel() = default;
        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        juce::Label titreLabel;
        CustomLookAndFeel& look;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
    };
}