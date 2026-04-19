#pragma once

#include <juce_audio_basics/juce_audio_basics.h> // audiobuffer and midibuffer
#include <juce_audio_processors/juce_audio_processors.h> // audio processor
#include <juce_core/juce_core.h> // memory block and string

#include "framework/bridge/FromUI.h"
#include "framework/bridge/FromAudio.h"
#include "framework/bridge/PingPongBuffer.h"
#include "framework/core/PluginTypes.h"
#include "framework/state/AudioState.h"
#include "framework/state/ParameterState.h"
#include "framework/state/UIState.h"
#include "utils/struct/UIContext.h"
#include "utils/struct/VisualSnapshot.h"

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
        ValueTreeState apvts;
        AudioState audioState;
        ParameterState paramState;
        UIState uiState;
        FromUI fui;
        FromAudio faudio;
        UIContext uic;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiAppProcessor)
    };

}