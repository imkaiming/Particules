#pragma once

#include "../../framework/GuiTypes.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
//#include <juce_gui_basics/juce_gui_basics.h>

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    class ADSRPanel : public juce::Component
    {
    public:
        ADSRPanel(UIContext& uic);
        ~ADSRPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        UIContext& uic;

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

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRPanel)
    };
}