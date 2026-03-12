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

#include "VoiceManager.h"
#include "Scheduler.h"


class ParameterView;
class GranularEngine
{
public:
    explicit GranularEngine(ParameterView& sp, GrainVisualBuffer& vb);
	~GranularEngine() = default;

	void process(juce::AudioBuffer<float>& output, int bufferSize);
	void init(double sampleRate, int numChannel, int samplePerBlocks);
    int getNumActiveGrains() const noexcept { return pool.getNumActiveGrains(); };

private:
	//void mixingProcess(AudioBlock);
	void gainProcess(juce::dsp::ProcessContextReplacing<float>);
	//void reverbProcess(juce::dsp::ProcessContextReplacing<float>);

	static constexpr uint8_t mMaxEvent = Param::MaxEvents;

    const float refreshRate;
    int accumulator;
    int threshold;
	
	EnvelopeLookUpTable envLut;
	ParameterView& paramsView;
    PositionModulator posMod;
	Scheduler scheduler;
    GrainPool pool;
	VoiceManager voiceManager;

	//juce::dsp::DryWetMixer<float> mixerProcessor;
	juce::dsp::Gain<float> gainProcessor;
	//juce::dsp::Reverb reverbProcessor;
	//juce::dsp::Reverb::Parameters params;

	//juce::AudioBuffer<float> phaseVocoderBuffer;
	//juce::dsp::WindowingFunction<float> window;
	//juce::dsp::FFT fft;
	//audiofft::AudioFFT fft;

};