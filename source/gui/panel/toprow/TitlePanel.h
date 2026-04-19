#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/GuiTypes.h"
#include "gui/component/button/IconButton.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    class FromUI;
    class AudioState;
    class UIState;
    class TitlePanel : public juce::Component, private juce::ChangeListener
    {
    public:
        TitlePanel(UIContext& uic);
        ~TitlePanel();

        void paint(juce::Graphics&) override;
        void resized() override;

        void changeListenerCallback(juce::ChangeBroadcaster*);

    private:

        juce::Label titleLabel, versionLabel;

        FromUI& fui;
        const AudioState& audioState;
        UIState& uiState;

        IconButton loadBtn;
        IconButton playBtn;
        IconButton previousBtn;
        IconButton nextBtn;
        std::unique_ptr<juce::Drawable> playIcon, pauseIcon, loadIcon, previousIcon, nextIcon;

        void pauseButtonClicked();
        void playButtonClicked();
        void loadSampleButtonClicked();

        bool lastPlayState;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
    };
}