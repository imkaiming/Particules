#pragma once

#include "../../framework/Constants.h"
#include "../../framework/Core.h"
#include "../../utils/struct/UIContext.h"
#include "../../utils/PluginParams.h"

namespace particules
{

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

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decaySliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseSliderAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRPanel)
    };
}