
#pragma once

#include "PositionOverlayComponent.h"
#include "SelectionOverlayComponent.h"
#include "GrainVisualComponent.h"

#include "../../framework/Types.h"
#include "../../framework/Core.h"
#include "../../utils/struct/UIContext.h"
#include "../../utils/PluginParams.h"

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

        //void setFile(const juce::File&);
        void setCallbackOnThumbnailReady(std::function<void()>);
        //void setNumSamples(const int);

    private:
        static constexpr const char* globalPositionId = global::position::id;
        static constexpr const char* globalSelectionId = global::selection::id;

        void paint(juce::Graphics&) override;
        void resized() override;

        void paintIfNoFileLoaded(juce::Graphics&);
        void paintIfFileLoaded(juce::Graphics&);

        void changeListenerCallback(juce::ChangeBroadcaster*) override; // to change the waveform when changing the audio
        //void parameterChanged(const juce::String& parameterID, float newValue) override;

        void updateOverflow(float value);
        void updatePosition(float value); // update the position marker when changing the file position slider value
        void updateSelection(float value);

        UIContext& uic;
        ParameterView& paramsView;
        ValueTreeState& apvts;
        ParticulesAudioProcessor& audioProcessor;

        float positionValue; 
        float selectionValue;

        //juce::AudioThumbnailCache& cache;
        juce::AudioThumbnail& audioThumbnail;

        std::function<void()> onThumbnailReady;

        PositionOverlayComponent position;
        SelectionOverlayComponent selection;
        SelectionOverlayComponent overflow; // when the selection is higher than the width it wrapped back. it add flexibility

        GrainVisualComponent grainVisualComponent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
    };
}