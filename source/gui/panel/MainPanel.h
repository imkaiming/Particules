#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../framework/Core.h"
#include "../../utils/struct/UIContext.h"

//#include "../Component/effect/VignetteEffect.h"

#include "botrow/TabPanel.h"
#include "toprow/AudioFilePanel.h"
#include "toprow/TitlePanel.h"

namespace particules
{

    struct UIContext;
    class MainPanel : public juce::Component
    {
    public:
        MainPanel(UIContext& uic);
        ~MainPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        TitlePanel titlePanel;
        AudioFilePanel audioFilePanel;
        TabPanel tabPanel;

        //VignetteEffect vignette;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainPanel)
    };
}
