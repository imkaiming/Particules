#pragma once

#include "../../framework/Types.h"

#include "../component/ThumbnailComponent.h"
#include "../../utils/PluginParams.h"

// the Synth Frame provide UI control that modify the apvts values only
namespace particules
{

    class SynthPanel: public juce::Component
    {
    public:
        SynthPanel(ValueTreeState& apvts);
        ~SynthPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:

        juce::Slider mixSlider;
        juce::Slider outputSlider;
        juce::Slider positionSlider;
        juce::Slider selectionSlider;

        juce::Label mixLabel;
        juce::Label outputLabel;
        juce::Label positionLabel;
        juce::Label selectionLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> positionSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> selectionSliderAttachment;

        // constants 
        static constexpr const char* globalMixId = global::mix::id;
        static constexpr const char* globalMixName = global::mix::name;
        static constexpr const float globalMixMin = global::mix::min;
        static constexpr const float globalMixMax = global::mix::max;

        static constexpr const char* globalPositionId = global::position::id;
        static constexpr const char* globalPositionName = global::position::name;
        static constexpr const float globalPositionMin = global::position::min;
        static constexpr const float globalPositionMax = global::position::max;

        static constexpr const char* globalSelectionId = global::selection::id;
        static constexpr const char* globalSelectionName = global::selection::name;
        static constexpr const float globalSelectionMin = global::selection::min;
        static constexpr const float globalSelectionMax = global::selection::max;

        static constexpr const char* globalOutputId = global::output::id;
        static constexpr const char* globalOutputName = global::output::name;
        static constexpr const float globalOutputMin = global::output::min;
        static constexpr const float globalOutputMax = global::output::max;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthPanel)
    };
}