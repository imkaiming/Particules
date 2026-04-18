// http://www.rossbencina.com/static/code/granular-synthesis/BencinaAudioAnecdotes310801.pdf

// Top level container that orchestrate all the blocks

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

#include "GrainEnvelope.h"
#include "GrainPool.h"
#include "GrainProcessor.h"
#include "PositionModulator.h"
#include "Scheduler.h"
#include "framework/PluginParams.h"
#include "framework/PluginTypes.h"
#include "framework/bridge/PingPongBuffer.h"
#include "utils/AtomicSharedPtr.h"
#include "utils/struct/SmoothedParameters.h"
#include "utils/struct/VisualSnapshot.h"

namespace particules
{
    struct ParameterSnapshot;
    class AudioState;
    class GranularEngine
    {
    public:
        GranularEngine(PingPongBuffer<VisualSnapshot>& vb, AudioState& as);
        ~GranularEngine() = default;

        void process(AudioBuffer& outputBuffer, AudioBuffer& inputBuffer, int bufferSize, float* const* outputPtrs,
            int outputNumChannels, const ParameterSnapshot& ps);

        void init(double, int, int);
        void clear() noexcept;
        int getNumActiveGrains() const noexcept { return pool.getNumActiveGrains(); }

    private:
        void writeVisualSnapshot() noexcept;
        void gainProcess(juce::dsp::ProcessContextReplacing<float>, const float);
        void updateSmoothedParameters() noexcept;
        void setTargetSmoothedValue(const ParameterSnapshot&) noexcept;

        static constexpr uint8_t mMaxEvent = params::maxSpawnsPerBlock;

        // core data members
        const float refreshRate;
        int sampleAccumulator;
        int threshold;
        std::function<void(const ParameterSnapshot& ps)> spawnCallback;

        // core components
        GrainEnvelope envLut;
        PositionModulator posMod;
        Scheduler scheduler;
        GrainPool pool;
        GrainProcessor grainProcessor;

        // processors
        juce::dsp::Gain<float> gainProcessor;
        juce::ADSR adsr;
        juce::ADSR::Parameters adsrParams;

        // smoothed value : we only smooth parameters that change the life cycle of the grains.
        juce::SmoothedValue<float> speedSmooth;
        //juce::SmoothedValue<float> sustainRatioSmooth;
        SmoothedParameters smoothedParams;

        AudioState& audioState;
        PingPongBuffer<VisualSnapshot>& visualBuffer;
    };
}
//juce::dsp::DryWetMixer<float> mixerProcessor;
//juce::dsp::Reverb reverbProcessor;
//juce::dsp::Reverb::Parameters params;

//juce::AudioBuffer<float> phaseVocoderBuffer;
//juce::dsp::WindowingFunction<float> window;
//juce::dsp::FFT fft;
//audiofft::AudioFFT fft;