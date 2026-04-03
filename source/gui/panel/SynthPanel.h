#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

#include "../../framework/GuiTypes.h"

#include "../../utils/PluginParams.h"
#include "../component/ThumbnailComponent.h"

// the Synth Frame provide UI control that modify the apvts values only

namespace juce
{
    class Graphics;
    //class AudioProcessorValueTreeState::SliderAttachment;
}

namespace particules
{
    struct UIContext;
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