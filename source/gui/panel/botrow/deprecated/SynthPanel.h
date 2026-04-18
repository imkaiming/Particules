#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/GuiTypes.h"
#include "framework/core/PluginParams.h"
#include "gui/component/ThumbnailComponent.h"
#include "gui/component/VerticalMeterComponent.h"

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

        void updateMeter(float);

    private:
        VerticalMeterComponent meter;

        juce::Slider outputSlider;
        juce::Label outputLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputSliderAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthPanel)
    };
}