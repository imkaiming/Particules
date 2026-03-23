/*
  ==============================================================================

	GranularEngine.h
	Created: 15 Feb 2023 1:44:09pm
	Author:  user

  ==============================================================================
*/

// http://www.rossbencina.com/static/code/granular-synthesis/BencinaAudioAnecdotes310801.pdf

// Top level container that orchestrate all the blocks

#pragma once

#include "../framework/Constants.h"
#include "../framework/Types.h"
#include "../utils/AtomicSharedPtr.h"
#include "../utils/struct/ParameterSnapshot.h"
#include "../utils/struct/SmoothedParameters.h"
#include "Scheduler.h"
#include "VoiceManager.h"

namespace particules
{
    class GranularEngine
    {
    public:
        GranularEngine(GrainVisualBuffer& vb);
        ~GranularEngine() = default;

        void process(
            AudioBuffer& output, int bufferSize, float* const* outputPtrs, int outputNumChannels, ParameterSnapshot snapshot);
        void init(double, int, int);
        int getNumActiveGrains() const noexcept { return pool.getNumActiveGrains(); }

        void setInputBuffer(std::shared_ptr<const AudioBuffer> ptr) noexcept { inputBufferPtr.store(std::move(ptr)); }
        std::shared_ptr<const AudioBuffer> getInputBuffer() const noexcept { return inputBufferPtr.load(); }
        const bool isInputBufferLoaded() const noexcept { return inputBufferPtr.load() != nullptr; }

    private:
        void gainProcess(juce::dsp::ProcessContextReplacing<float>, const float);
        void updateSmoothedParameters() noexcept;
        void setTargetSmoothedValue(const ParameterSnapshot&) noexcept;

        static constexpr uint8_t mMaxEvent = maxSpawnsPerBlock;

        AtomicSharedPtr<const AudioBuffer> inputBufferPtr; // should be downmixed
        //std::shared_ptr<const AudioBuffer> inputBuffer;
        std::function<void(const ParameterSnapshot& s)> spawnCallback;

        const float refreshRate;
        int sampleAccumulator;
        int threshold;

        GrainEnvelope envLut;
        PositionModulator posMod;
        Scheduler scheduler;
        GrainPool pool;
        VoiceManager voiceManager;

        juce::dsp::Gain<float> gainProcessor;

        juce::ADSR adsr;
        juce::ADSR::Parameters adsrParams;
        // smoothed value : we only smooth parameters that change the life cycle of the grains.
        juce::SmoothedValue<float> speedSmooth;
        //juce::SmoothedValue<float> sustainRatioSmooth;
        SmoothedParameters smoothedParams;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GranularEngine)
    };
}
//juce::dsp::DryWetMixer<float> mixerProcessor;
//juce::dsp::Reverb reverbProcessor;
//juce::dsp::Reverb::Parameters params;

//juce::AudioBuffer<float> phaseVocoderBuffer;
//juce::dsp::WindowingFunction<float> window;
//juce::dsp::FFT fft;
//audiofft::AudioFFT fft;