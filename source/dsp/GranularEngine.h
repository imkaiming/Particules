#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

#include "GrainEnvelope.h"
#include "GrainPool.h"
#include "GrainProcessor.h"
#include "PositionModulator.h"
//#include "Scheduler.h"
#include "VoiceManager.h"
#include "framework/bridge/PingPongBuffer.h"
#include "framework/core/PluginParams.h"
#include "framework/core/PluginTypes.h"
#include "utils/AtomicSharedPtr.h"
#include "utils/struct/SmoothedParameters.h"
#include "utils/struct/VisualSnapshot.h"

// http://www.rossbencina.com/static/code/granular-synthesis/BencinaAudioAnecdotes310801.pdf
// Top level container that orchestrate all the blocks

namespace particules
{
    struct ParameterSnapshot;
    struct AudioPayload;
    class FromAudio;
    class GranularEngine
    {
    public:
        GranularEngine(FromAudio& fa);
        ~GranularEngine() = default;

        void process(AudioBuffer& outputBuffer, juce::MidiBuffer& midiBuffer, AudioPayload* payload, int bufferSize,
            float* const* outputPtrs,
            int outputNumChannels, const ParameterSnapshot& ps);

        void init(double, int, int);
        void clear() noexcept;
        int getNumActiveGrains() const noexcept { return pool.getNumActiveGrains(); }

    private:
        void gainProcess(juce::dsp::ProcessContextReplacing<float>, const float);
        void updateSmoothedParameters() noexcept;
        void setTargetSmoothedValue(const ParameterSnapshot&) noexcept;

        //static constexpr uint8_t mMaxEvent = params::maxSpawnsPerBlock;

        // core data members
        const float refreshRate;
        int sampleAccumulator;
        int threshold;

        //SpawnGrainCallback spawnCallback;

        // core components
        GrainEnvelope envLut;
        PositionModulator posMod;
        VoiceManager voiceManager;
        GrainPool pool;
        GrainProcessor grainProcessor;

        // processors
        //juce::ADSR adsr;
        //juce::ADSR::Parameters adsrParams;

        // smoothed value : we only smooth parameters that change the life cycle of the grains.
        //juce::SmoothedValue<float> speedSmooth;
        //juce::SmoothedValue<float> attackSmooth;
        //juce::SmoothedValue<float> decaySmooth;
        //juce::SmoothedValue<float> sustainSmooth;
        //juce::SmoothedValue<float> releaseSmooth;
        //juce::SmoothedValue<float> sustainRatioSmooth;

        SmoothedParameters smoothedParams;
        juce::dsp::Gain<float> gainProcessor;

        FromAudio& faudio;
    };
}
//juce::dsp::DryWetMixer<float> mixerProcessor;
//juce::dsp::Reverb reverbProcessor;
//juce::dsp::Reverb::Parameters params;

//juce::AudioBuffer<float> phaseVocoderBuffer;
//juce::dsp::WindowingFunction<float> window;
//juce::dsp::FFT fft;
//audiofft::AudioFFT fft;