#pragma once

#include "dsp/GranularEngine.h"
#include "framework//core/Core.h"
#include "framework/audio/AudioFileLoader.h"
#include "framework/bridge/FromAudio.h"
#include "framework/bridge/FromUI.h"
#include "framework/bridge/PingPongBuffer.h"
#include "framework/bridge/RingBuffer.h"
#include "framework/bridge/StateSynchronizer.h"
#include "framework/core/PluginTypes.h"
#include "framework/state/AudioState.h"
#include "framework/state/ParameterState.h"
#include "framework/state/UIState.h"
#include "utils/struct/AudioPayload.h"
#include "utils/struct/UIContext.h"

namespace juce
{
    class AudioProcessor;
}

namespace particules
{

    class PluginCore
    {
    public:
        PluginCore(juce::AudioProcessor& parentProcessor);
        ~PluginCore();

        void prepareToPlay(double sampleRate, int samplesPerBlock);
        void processBlock(AudioBuffer&, juce::MidiBuffer&);
        void releaseResources();

        void getStateInformation(juce::MemoryBlock& destData);
        void setStateInformation(const void* data, int sizeInBytes);

        //ParameterState& getParametersView() noexcept { return paramsView; };
        ValueTreeState& getAPVTS() noexcept { return apvts; };
        UIContext& getUIContext() noexcept { return uic; };
        juce::MidiKeyboardState& getKeyboardState() noexcept { return keyboardState; }
        ////const juce::File& getCurrentFile() const noexcept { return loader.getCurrentFile(); };
        //AudioFileLoader& getAudioFileLoader() noexcept { return loader; };
        //const int getNumActiveGrains() const noexcept { return granularEngine.getNumActiveGrains(); };
        //const bool isInputBufferLoaded() const noexcept { return granularEngine.isInputBufferLoaded(); };

        // accessors
        //ValueTreeState& getAPVTS() noexcept { return apvts; }
        //UIState& getUIState() noexcept { return uiState; }
        //AudioState& getAudioState() noexcept { return audioState; }
        //AudioFileLoader& getAudioFileLoader() noexcept { return loader; }
        void loadDebugPreset();

    private:
        static ValueTreeState::ParameterLayout createParameterLayout();
        bool debugPresetLoaded;

        juce::AudioProcessor& proc;

        // global states manager
        AudioState audioState;
        UIState uiState;

        // lock free queues
        RingBuffer<AudioPayload*> incomingBuffer;
        RingBuffer<AudioPayload*> garbageCollector;
        PingPongBuffer<VisualSnapshot> visualBuffer;
        std::atomic<AudioPayload*> currentPayload;

        // communications pipeline
        FromAudio faudio;
        FromUI fui;

        ValueTreeState apvts;
        ParameterState paramState;
        GranularEngine granularEngine;
        AudioFileLoader loader;
        UIContext uic;

        // is called after the audio file loader has successfully
        // load a sample to init all the audio related component
        // engine state and uistate are updated and uistate will
        // broadcast the message to the UI component
        AudioLoadedCallback onAudioLoadedCallback;

        // timer for thread communication
        StateSynchronizer synchronizer;

        bool wasAuditioning;

        juce::MidiKeyboardState keyboardState;
    };
}