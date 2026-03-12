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

#include "../utils/AtomicSharedPtr.h"
#include "../utils/ParameterSnapshot.h"
#include "Scheduler.h"
#include "VoiceManager.h"

class GranularEngine
{
public:
    explicit GranularEngine(GrainVisualBuffer& vb);
    ~GranularEngine() = default;

    void process(juce::AudioBuffer<float>& output, int bufferSize, ParameterSnapshot snapshot);
    void init(double sampleRate, int numChannel, int samplePerBlocks);
    int getNumActiveGrains() const noexcept { return pool.getNumActiveGrains(); };

    void setInputBuffer(std::shared_ptr<const AudioBuffer> ptr) noexcept { inputBuffer.store(std::move(ptr)); };
    std::shared_ptr<const AudioBuffer> getInputBuffer() const noexcept { return inputBuffer.load(); };
    const bool isInputBufferLoaded() const noexcept { return inputBuffer.load() != nullptr; };

private:
    void gainProcess(juce::dsp::ProcessContextReplacing<float>, const float);
    static constexpr uint8_t mMaxEvent = Param::MaxEvents;

    AtomicSharedPtr<const AudioBuffer> inputBuffer;
    //std::shared_ptr<const AudioBuffer> inputBuffer;

    const float refreshRate;
    int accumulator;
    int threshold;

    EnvelopeLookUpTable envLut;
    PositionModulator posMod;
    Scheduler scheduler;
    GrainPool pool;
    VoiceManager voiceManager;

    juce::dsp::Gain<float> gainProcessor;
};
//juce::dsp::DryWetMixer<float> mixerProcessor;
//juce::dsp::Reverb reverbProcessor;
//juce::dsp::Reverb::Parameters params;

//juce::AudioBuffer<float> phaseVocoderBuffer;
//juce::dsp::WindowingFunction<float> window;
//juce::dsp::FFT fft;
//audiofft::AudioFFT fft;