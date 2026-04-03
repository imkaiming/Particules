#pragma once

#include <juce_core/juce_core.h> // end macro
#include <juce_events/juce_events.h> // changeListener
#include <juce_gui_basics/juce_gui_basics.h> // component

#include "GrainVisualComponent.h"

#include "../../framework/Core.h"

#include "../../utils/PluginParams.h"

namespace juce
{
    class AudioThumbnail;
    class Graphics;
}

// Thumbnail is where the audio waveform is painted after being loaded by the audio file loader
// should not own the state of the plugin
namespace particules
{
    class EngineState;
    class UIState;
    class ParameterView;
    struct UIContext;
    class ParticulesAudioProcessor;
    class ThumbnailComponent : public juce::Component,
                               private juce::ChangeListener //, public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        ThumbnailComponent(UIContext&);
        ~ThumbnailComponent() override;

        void setCallbackOnThumbnailReady(std::function<void()>);

    private:
        static constexpr const char* globalPositionId = global::position::id;
        static constexpr const char* globalSpanId = global::span::id;

        void paint(juce::Graphics&) override;
        void resized() override;

        void paintIfNoFileLoaded(juce::Graphics&);
        void paintIfFileLoaded(juce::Graphics&);
        void paintGrid(juce::Graphics&);

        void changeListenerCallback(juce::ChangeBroadcaster*) override; // to change the waveform when changing the audio
        //void parameterChanged(const juce::String& parameterID, float newValue) override;

        EngineState& engineState;
        UIState& uiState;
        //ValueTreeState& apvts;

        juce::AudioThumbnail& audioThumbnail;

        std::function<void()> onThumbnailReady;

        GrainVisualComponent grainVisualComponent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
    };
}