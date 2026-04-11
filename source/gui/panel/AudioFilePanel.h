#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../framework/GuiTypes.h"
#include "../../framework/PluginParams.h"
#include "../component/ThumbnailComponent.h"
#include "../component/overlay/WaveformOverlay.h"

// the Audio File Frame provide the control to load and play the audio.
namespace particules
{
    class UIState;
    struct UIContext;
    class AudioFilePanel : public juce::Component, public juce::FileDragAndDropTarget //, public juce::ChangeListener
    {
    public:
        AudioFilePanel(UIContext& uic);
        ~AudioFilePanel() = default;
        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        //void changeListenerCallback(juce::ChangeBroadcaster*) override;
        bool isInterestedInFileDrag(const juce::StringArray&);
        void filesDropped(const juce::StringArray&, int, int);

        UIState& uiState;
        UIContext& uic;
        ValueTreeState& apvts;
        ThumbnailComponent thumbnailComponent; // after the file is loaded draw the waveform

        //juce::Slider positionSlider;
        //juce::Slider spanSlider;

        //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> positionSliderAttachment;
        //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> spanSliderAttachment;

        //void updateOverflow(float value);
        //void updatePosition(float value); // update the position marker when changing the file position slider value
        //void updateSpan(float value);

        std::unique_ptr<WaveformOverlay> waveformOverlay;


        static constexpr const char* globalPositionId = params::position::id;
        static constexpr const char* globalPositionName = params::position::name;
        static constexpr const float globalPositionMin = params::position::min;
        static constexpr const float globalPositionMax = params::position::max;

        static constexpr const char* globalSpanId = params::span::id;
        static constexpr const char* globalSpanName = params::span::name;
        static constexpr const float globalSpanMin = params::span::min;
        static constexpr const float globalSpanMax = params::span::max;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFilePanel)
    };

}