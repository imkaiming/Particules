#pragma once

#include <juce_audio_basics/juce_audio_basics.h> // audiobuffer and midibuffer
#include <juce_audio_processors/juce_audio_processors.h> // audio processor
#include <juce_core/juce_core.h> // memory block and string

//#include "framework/audio/AudioFileLoader.h"
#include "../framework/PluginTypes.h"
#include "../framework/bridge/EngineState.h"
#include "../framework/bridge/GrainVisualBuffer.h"
#include "../framework/bridge/ParameterView.h"
#include "../framework/bridge/UIState.h"
#include "../utils/struct/ProcessorFacade.h"
#include "../utils/struct/UIContext.h"

// just to test GUI separated than the audio DSP
namespace particules
{
    inline std::unique_ptr<juce::AudioParameterFloat> createNormalizedParameter(const juce::ParameterID& id,
        const str& name, float min, float max, float skew, float init,
        std::function<str(float, int)> stringFromValueFunc,
        std::function<float(const str&)> valueFromStringFunc)
    {
        //const float internalMax = max - min;
        //const float internalSkewCentre = skew - min;
        //const float internalInit = init - min;

        juce::NormalisableRange<float> range{min, max};
        range.setSkewForCentre(skew);

        return std::make_unique<juce::AudioParameterFloat>(id, name, range, init,
            juce::AudioParameterFloatAttributes{}
                .withCategory(juce::AudioProcessorParameter::genericParameter)
                .withStringFromValueFunction(stringFromValueFunc)
                .withValueFromStringFunction(valueFromStringFunc));
    }

    class GuiAppProcessor : public juce::AudioProcessor
    {
    public:
        GuiAppProcessor();
        ~GuiAppProcessor() = default;

        // Audio Processor classes //
        void prepareToPlay(double sampleRate, int samplesPerBlock) override;
        void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

        void processBlock(AudioBuffer&, juce::MidiBuffer&) override;
        juce::AudioProcessorEditor* createEditor() override;
        bool hasEditor() const override;
        const str getName() const override;
        bool acceptsMidi() const override;
        bool producesMidi() const override;
        bool isMidiEffect() const override;
        double getTailLengthSeconds() const override;
        int getNumPrograms() override;
        int getCurrentProgram() override;
        void setCurrentProgram(int index) override;
        const str getProgramName(int index) override;
        void changeProgramName(int index, const str& newName) override;
        void getStateInformation(juce::MemoryBlock& destData) override;
        void setStateInformation(const void* data, int sizeInBytes) override;

        static ValueTreeState::ParameterLayout createParameterLayout();
        UIContext& getUIContext() noexcept { return uic; };

    private:
        GrainVisualBuffer visualBuffer;
        ValueTreeState apvts;
        EngineState engineState;
        ParameterView paramsView;
        UIState uiState;

        ProcessorFacade facade;
        UIContext uic;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiAppProcessor)
    };

}