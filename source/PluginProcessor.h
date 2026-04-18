#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

#include "dsp/GranularEngine.h"
#include "framework/PluginTypes.h"
#include "framework/audio/AudioFileLoader.h"
#include "framework/bridge/AudioState.h"
#include "framework/bridge/ParameterView.h"
#include "framework/bridge/PingPongBuffer.h"
#include "framework/bridge/RingBuffer.h"
#include "framework/bridge/StateSynchronizer.h"
#include "framework/bridge/UIState.h"
#include "utils/struct/ProcessorFacade.h"
#include "utils/struct/UIContext.h"
#include "utils/struct/VisualSnapshot.h"

//#include "framework/PluginCore.h"

namespace juce
{
    class File;
}

namespace particules
{
    inline std::unique_ptr<juce::AudioParameterFloat> createNormalizedParameter(const juce::ParameterID& id, const str& name,
        float min, float max, float skew, float init, std::function<str(float, int)> stringFromValueFunc,
        std::function<float(const str&)> valueFromStringFunc)
    {
        juce::NormalisableRange<float> range{min, max};
        range.setSkewForCentre(skew);

        return std::make_unique<juce::AudioParameterFloat>(id, name, range, init,
            juce::AudioParameterFloatAttributes{}
                .withCategory(juce::AudioProcessorParameter::genericParameter)
                .withStringFromValueFunction(stringFromValueFunc)
                .withValueFromStringFunction(valueFromStringFunc));
    }

    inline std::unique_ptr<juce::AudioParameterFloat> createNormalizedParameter(
        const juce::ParameterID& id, const str& name, float min, float max, float skew = 0.5f, float init = 0.5f)
    {
        juce::NormalisableRange<float> range{min, max};
        range.setSkewForCentre(skew);

        return std::make_unique<juce::AudioParameterFloat>(id, name, range, init);
    }

    class GranularEngine;
    class ParticulesAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
        ,
                                     public juce::AudioProcessorARAExtension
#endif
    {
    public:
        ParticulesAudioProcessor();
        ~ParticulesAudioProcessor() override;

        // Audio Processor classes //
        void prepareToPlay(double sampleRate, int samplesPerBlock) override;
        void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
        bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

        void processBlock(AudioBuffer&, juce::MidiBuffer&) override;

        juce::AudioProcessorEditor* createEditor() override;
        bool hasEditor() const override;
        const juce::String getName() const override;
        bool acceptsMidi() const override;
        bool producesMidi() const override;
        bool isMidiEffect() const override;
        double getTailLengthSeconds() const override;
        int getNumPrograms() override;
        int getCurrentProgram() override;
        void setCurrentProgram(int index) override;
        const juce::String getProgramName(int index) override;
        void changeProgramName(int index, const str& newName) override;

        void getStateInformation(juce::MemoryBlock& destData) override;
        void setStateInformation(const void* data, int sizeInBytes) override;

        // User classes //

        ValueTreeState& getValueTreeState() noexcept { return apvts; };
        ParameterView& getParametersView() noexcept { return paramsView; };
        UIContext& getUIContext() noexcept { return uic; };
        //const juce::File& getCurrentFile() const noexcept { return loader.getCurrentFile(); };
        AudioFileLoader& getAudioFileLoader() noexcept { return loader; };
        const int getNumActiveGrains() const noexcept { return granularEngine.getNumActiveGrains(); };
        //const bool isInputBufferLoaded() const noexcept { return granularEngine.isInputBufferLoaded(); };

    private:
        static ValueTreeState::ParameterLayout createParameterLayout();
        void loadDebugPreset();
        void loadFile(const str& path);
        void loadFile();

        bool debugPresetLoaded = false;

        // thread communication
        PingPongBuffer<VisualSnapshot> visualBuffer;
        RingBuffer<AudioPayload> incomingBuffer;
        RingBuffer<AudioPayload> garbageCollector;
        std::atomic<AudioPayload*> currentPayload{nullptr};

        // core components
        ValueTreeState apvts; // connecte les slider du GUI et les paramètres (fourni des valeurs atomiques)
        AudioState audioState; // own runtime plugin global parameters no snapshot
        ParameterView paramsView; // fait le pont entre apvts et le synth
        UIState uiState; //
        GranularEngine granularEngine;
        AudioFileLoader loader;
        ProcessorFacade facade;
        UIContext uic;
        StateSynchronizer synchronizer;

        // is called after the audio file loader has successfully
        // load a sample to init all the audio related component
        // engine state and uistate are updated and uistate will
        // broadcast the message to the UI component
        AudioLoadedCallback onAudioLoadedCallback;

        //std::function<void(std::shared_ptr<const AudioBuffer>)> setInputBufferCallback;
        //juce::UndoManager undoManager;
        //juce::AbstractFifo fifo;
        //juce::ADSR::Parameters adsrParameters;
        //juce::ADSR adsr;

        //PluginCore core;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessor)
    };
}
