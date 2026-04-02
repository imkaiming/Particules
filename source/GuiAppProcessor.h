#pragma once

#include "framework/Types.h"
#include "framework/audio/AudioFileLoader.h"
#include "framework/bridge/EngineState.h"
#include "framework/bridge/GrainVisualBuffer.h"
#include "framework/bridge/ParameterView.h"
#include "framework/bridge/UIState.h"
#include "framework/Core.h"
#include "utils/struct/UIContext.h"

// just to test GUI separated than the audio DSP
namespace particules
{

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

        void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
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
        //void loadFile(const str& path);
        //void loadFile();
        //void initOnAudioLoadedCallback();

        GrainVisualBuffer visualBuffer;
        ValueTreeState apvts;
        EngineState engineState;
        ParameterView paramsView;
        UIState uiState;
        UIContext uic;

        AudioFileLoader loader;
        //AudioLoadedCallback onAudioLoadedCallback;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiAppProcessor)
    };
}