#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/GuiTypes.h"
#include "gui/component/button/IconButton.h"
#include "gui/component/rotarymenu/EnvelopeRotaryMenu.h"
#include "gui/component/rotarymenu/TraversalRotaryMenu.h"
#include "gui/component/slider/MainSliderWithAux.h"
#include "gui/component/slider/RotarySlider.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    class AudioState;
    class GrainsPanel : public juce::Component //, private ValueTreeState::Listener
    {
    public:
        GrainsPanel(UIContext& uic);
        ~GrainsPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        //void parameterChanged(const juce::String& parameterID, float newValue) override;
        void setLinkButtonImage();
        void onPlaybackButtonClicked();

        AudioState& audioState;
        ValueTreeState& apvts;

        MainSliderWithAux emissionSlider;
        MainSliderWithAux durationSlider;
        MainSliderWithAux speedSlider;
        MainSliderWithAux panSlider;

        IconButton linkBtn;
        IconButton playbackBtn;
        std::unique_ptr<juce::Drawable> linkInIcon, linkOffIcon, forwardIcon, backwardIcon;
        std::atomic<float>* playback;

        EnvelopeRotaryMenu envelopeRotaryMenu;
        TraversalRotaryMenu traversalRotaryMenu;

        juce::Slider attackSlider;
        juce::Slider decaySlider;
        juce::Slider sustainSlider;
        juce::Slider releaseSlider;

        juce::Label attackLabel;
        juce::Label decayLabel;
        juce::Label sustainLabel;
        juce::Label releaseLabel;

        std::unique_ptr<ValueTreeState::SliderAttachment> attackSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> decaySliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> sustainSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> releaseSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> emissionSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> durationSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> speedSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> panSliderAttachment;

        SecondaryRotarySlider outputSlider;
        std::unique_ptr<ValueTreeState::SliderAttachment> outputSliderAttachment;
        juce::Label outputLabel;

        bool isLinkingUpdate ;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsPanel)
    };
}