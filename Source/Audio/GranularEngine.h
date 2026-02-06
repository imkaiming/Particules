/*
  ==============================================================================

	GranularEngine.h
	Created: 15 Feb 2023 1:44:09pm
	Author:  user

  ==============================================================================
*/

// http://www.rossbencina.com/static/code/granular-synthesis/BencinaAudioAnecdotes310801.pdf

// Top level container responsible for the lifetime of its component parts.
// Mediates between external audio streaming services, the Schedulerand possibly the SourceData,
// for example a Delay Line in the case of a Delay Line Granulator.
// Manages dynamic parameter modulation or acts as a Facade7 to allow clients to modulate
// synthesis parameters.

#pragma once

#include <juce_dsp/juce_dsp.h>
#include "Scheduler.h"
#include "../Utils/CircularBuffer.h"
#include "../Utils/StateParameters.h"
#include "../FrameWork/Core.h"


class GranularEngine
{
public:
	GranularEngine();
	~GranularEngine();

	void setStateParameters(StateParameters* sp);
	void process(juce::AudioBuffer<float>& buffer, int numSamples);
	void init(StateParameters* sp, int sampleRate, int numChannel, int samplePerBlocks);

private:
	void mixingProcess(AudioBlock);
	void gainProcess(juce::dsp::ProcessContextReplacing<float>);
	void reverbProcess(juce::dsp::ProcessContextReplacing<float>);

	Scheduler scheduler;
	StateParameters* stateParams;

	//CircularBuffer circularBuffer;
	//juce::AudioBuffer<float> phaseVocoderBuffer;
	//juce::dsp::WindowingFunction<float> window;
	//juce::dsp::FFT fft;
	audiofft::AudioFFT fft;

	juce::dsp::DryWetMixer<float> mixerProcessor;

	juce::dsp::Gain<float> gainProcessor;
	juce::dsp::Reverb reverbProcessor;
	juce::dsp::Reverb::Parameters params;

};