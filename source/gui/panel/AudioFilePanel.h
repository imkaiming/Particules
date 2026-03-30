
#pragma once

#include "../../framework/Types.h"
#include "../component/ThumbnailComponent.h"
#include "../component/HandleSliderComponent.h"

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

        HandleSliderComponent positionSlider;
        HandleSliderComponent spanSlider;

        //juce::Label positionLabel;
        //juce::Label spanLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> positionSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> spanSliderAttachment;

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