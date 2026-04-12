#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../../framework/GuiTypes.h"
#include "../../component/button/IconButton.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    struct ProcessorFacade;
    class EngineState;
    class TitlePanel : public juce::Component
    {
    public:
        TitlePanel(UIContext& uic);
        ~TitlePanel();

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        //static constexpr const char* fileNameBoxPlaceHolder = "audio_file_03.wav";

        UIContext& uic;

        juce::Label titleLabel;//, fileNameBox;

        ProcessorFacade& facade;
        EngineState& engineState;

        IconButton loadBtn;
        IconButton playBtn;
        std::unique_ptr<juce::Drawable> playIcon, pauseIcon, loadIcon;

        //LabelBoxLookAndFeel lookAndFeel;

        void pauseButtonClicked();
        void playButtonClicked();
        void loadSampleButtonClicked();

        //void layoutLeft();
        //void layoutLoad();
        //void layoutFile();
        //void layoutRight();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
    };
}