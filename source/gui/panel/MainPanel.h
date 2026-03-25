#pragma once


#include "../../framework/Core.h"
#include "../../utils/struct/UIContext.h"

#include "AudioFilePanel.h"
#include "SynthPanel.h"
#include "GrainsPanel.h"
#include "TitlePanel.h"
#include "StatusBarPanel.h"

namespace particules
{

    //struct UIContext;
    class MainPanel : public juce::Component
    {
    public:
        MainPanel(UIContext& uic);
        ~MainPanel() = default;
        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        SynthPanel synthPanel;
        GrainsPanel grainsPanel;
        AudioFilePanel audioFilePanel;
        StatusBarPanel statusBarPanel;
        TitlePanel titlePanel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPanel)
    };
}