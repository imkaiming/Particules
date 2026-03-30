
#pragma once

#include "../../framework/Types.h"
#include "../../utils/lookandfeel/HandleSliderLookAndFeel.h"
#include "../component/ThumbnailComponent.h"
#include "../component/PositionOverlayComponent.h"
#include "../component/SpanOverlayComponent.h"

// the Audio File Frame provide the control to load and play the audio.
namespace particules
{

    class ParameterView;
    class ParticulesAudioProcessor;
    class AudioFilePanel : public juce::Component, public juce::FileDragAndDropTarget, public juce::ChangeListener
    {
    public:
        AudioFilePanel(UIContext& uic);
        ~AudioFilePanel();
        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        void changeListenerCallback(juce::ChangeBroadcaster*) override;
        bool isInterestedInFileDrag(const juce::StringArray&);
        void filesDropped(const juce::StringArray&, int, int);

        ParameterView& paramsView;
        ParticulesAudioProcessor& audioProcessor;
        ValueTreeState& apvts;
        ThumbnailComponent thumbnailComponent; // after the file is loaded draw the waveform

        juce::Slider positionSlider;
        juce::Slider spanSlider;

        HandleSliderLookAndFeel sliderLookAndFeel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> positionSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> spanSliderAttachment;

        void updateOverflow(float value);
        void updatePosition(float value); // update the position marker when changing the file position slider value
        void updateSpan(float value);

        PositionOverlayComponent positionOverlay;
        SpanOverlayComponent spanOverlay;
        SpanOverlayComponent overflowOverlay; // when the selection is higher than the width it wrapped back. it add flexibility

        static constexpr const char* globalPositionId = global::position::id;
        static constexpr const char* globalPositionName = global::position::name;
        static constexpr const float globalPositionMin = global::position::min;
        static constexpr const float globalPositionMax = global::position::max;

        static constexpr const char* globalSpanId = global::span::id;
        static constexpr const char* globalSpanName = global::span::name;
        static constexpr const float globalSpanMin = global::span::min;
        static constexpr const float globalSpanMax = global::span::max;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFilePanel)
    };

}