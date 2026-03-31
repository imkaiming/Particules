#pragma once

#include "../../framework/Types.h"

#include "../../utils/PluginParams.h"
#include "../component/ThumbnailComponent.h"

// the Synth Frame provide UI control that modify the apvts values only
namespace particules
{

    class SynthPanel : public juce::Component
    {
    public:
        SynthPanel(UIContext&);
        ~SynthPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        juce::Slider outputSlider;
        juce::Label outputLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputSliderAttachment;

        // constants
        static constexpr const char* globalOutputId = global::output::id;
        static constexpr const char* globalOutputName = global::output::name;
        static constexpr const float globalOutputMin = global::output::min;
        static constexpr const float globalOutputMax = global::output::max;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthPanel)
    };
}