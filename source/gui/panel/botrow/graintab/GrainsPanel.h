#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../../../framework/GuiTypes.h"
#include "../../../../framework/PluginParams.h"
#include "../../../component/button/IconButton.h"
#include "../../../component/rotarymenu/EnvelopeRotaryMenu.h"
#include "../../../component/rotarymenu/TraversalRotaryMenu.h"
#include "../../../component/menugroup/EnvelopeMenuControl.h"
#include "../../../component/menugroup/TraversalMenuControl.h"
#include "../../../component/slider/MainSliderWithAux.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    class EngineState;
    class GrainsPanel : public juce::Component //, private ValueTreeState::Listener
    {
    public:
        GrainsPanel(UIContext& uic);
        ~GrainsPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        //void parameterChanged(const juce::String& parameterID, float newValue) override;
        void linkButtonClicked();
        void setLinkButtonImage();

        EngineState& engineState;

        MainSliderWithAux emissionSlider;
        MainSliderWithAux durationSlider;
        MainSliderWithAux speedSlider;
        MainSliderWithAux panSlider;

        IconButton linkBtn;
        std::unique_ptr<juce::Drawable> linkInIcon, linkOffIcon;
        std::unique_ptr<ValueTreeState::SliderAttachment> emissionSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> durationSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> speedSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> panSliderAttachment;

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

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsPanel)
    };
}