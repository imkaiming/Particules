#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/GuiTypes.h"
#include "framework/core/PluginParams.h"
#include "gui/component/GrainVisualComponent.h"
#include "gui/component/ThumbnailComponent.h"
#include "gui/component/slider/SliderOnWaveform.h"

// the Audio File Frame provide the control to load and play the audio.
namespace particules
{
    class UIState;
    class AudioState;
    class FromUI;
    struct UIContext;
    class AudioFilePanel : public juce::Component,
                           public juce::FileDragAndDropTarget,
                           private juce::ChangeListener,
                           public juce::Timer
    {
    public:
        AudioFilePanel(UIContext& uic);
        ~AudioFilePanel() override;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        //void changeListenerCallback(juce::ChangeBroadcaster*) override;
        bool isInterestedInFileDrag(const juce::StringArray&);
        void filesDropped(const juce::StringArray&, int, int);

        void changeListenerCallback(juce::ChangeBroadcaster* source) override;
        void timerCallback() override;

        UIContext& uic;
        /*const*/ UIState& uiState;
        const AudioState& audioState;
        FromUI& fui;


        ValueTreeState& apvts;

        ThumbnailComponent thumbnailComponent; // after the file is loaded draw the waveform
        GrainVisualComponent grainVisualComponent;
        std::unique_ptr<SliderOnWaveform> sliderOnWaveform;

        juce::Label fileNameLabel;
        juce::Label grainsLabel;
        juce::Label numGrainsLabel;

        juce::Label posTitleLabel;
        juce::Label posLabel;

        juce::Label spanTitleLabel;
        juce::Label spanLabel;

        std::atomic<float>* posParam;
        std::atomic<float>* spanParam;

        int numSamples, lastNumGrains;
        float lastSpan, lastPos;
        //bool lastPlayState;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFilePanel)
    };

}