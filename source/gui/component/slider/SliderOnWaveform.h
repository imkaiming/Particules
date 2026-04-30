#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/GuiTypes.h"

// SliderOnWaveform is a component that define two custom user controls.
// it allows the user to interact with with the position and the span without
// defining sliders

namespace particules
{
    class SliderOnWaveform : public juce::Component
    {
    public:
        SliderOnWaveform(ValueTreeState& apvts, const juce::ParameterID& posId, const juce::ParameterID& spanId);

        void setAudioLoaded(bool loaded);

        void paint(juce::Graphics& g) override;

    private:

        enum class HoverState { None, Position, Body, Edge };
        enum class DragMode { None, Position, SpanBody, SpanEdge };

        HoverState currentHover;
        DragMode dragMode;

        HoverState getHoverStateAt(float mouseX) const;

        void mouseMove(const juce::MouseEvent& e) override;
        void mouseExit(const juce::MouseEvent&) override;
        void mouseDown(const juce::MouseEvent& e) override;
        void mouseDrag(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent&) override;

        float dragStartX;
        float dragStartPos;
        float dragStartSpan;

        std::atomic<float>* rawPosValue;
        std::atomic<float>* rawSpanValue;

        std::unique_ptr<juce::ParameterAttachment> positionAttachment;
        std::unique_ptr<juce::ParameterAttachment> spanAttachment;

        bool isAudioLoaded;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderOnWaveform)
    };
}