
#pragma once

#include "GrainVisualComponent.h"

#include "../../framework/Core.h"
#include "../../framework/Types.h"
#include "../../utils/PluginParams.h"
#include "../../utils/struct/UIContext.h"

// Thumbnail is where the audio waveform is painted after being loaded by the audio file loader
// should not own the state of the plugin
namespace particules
{

    class ParameterView;
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

        UIContext& uic;
        ParameterView& paramsView;
        ValueTreeState& apvts;
        ParticulesAudioProcessor& audioProcessor;

        //juce::AudioThumbnailCache& cache;
        juce::AudioThumbnail& audioThumbnail;

        std::function<void()> onThumbnailReady;

        GrainVisualComponent grainVisualComponent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
    };
}