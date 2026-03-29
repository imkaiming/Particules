#pragma once

#include "../../framework/Core.h"
#include "../../utils/struct/UIContext.h"

#include "AudioFilePanel.h"
#include "ControlPanel.h"
#include "StatusBarPanel.h"
#include "TitlePanel.h"

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
        //juce::Rectangle<int> withOuterPadding(const juce::Rectangle<int>& r, float ratio = 0.012f)
        //{
        //    int padding = juce::roundToInt(std::min(r.getWidth(), r.getHeight()) * ratio);
        //    return r.reduced(padding);
        //}


        TitlePanel titlePanel;
        AudioFilePanel audioFilePanel;
        ControlPanel controlPanel;
        StatusBarPanel statusBarPanel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPanel)
    };
}
