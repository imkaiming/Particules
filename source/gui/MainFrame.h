#pragma once


#include "../framework/Core.h"
#include "../utils/struct/UIContext.h"

#include "AudioFileFrame.h"
#include "SynthFrame.h"
#include "GrainsFrame.h"
#include "TitleFrame.h"
#include "StatusBarFrame.h"

namespace particules
{

    //struct UIContext;
    class MainFrame : public juce::Component
    {
    public:
        MainFrame(UIContext& uic);
        ~MainFrame() = default;
        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        SynthFrame synthFrame;
        GrainsFrame grainsFrame;
        AudioFileFrame audioFileFrame;
        StatusBarFrame statusBarFrame;
        TitleFrame titleFrame;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainFrame)
    };
}