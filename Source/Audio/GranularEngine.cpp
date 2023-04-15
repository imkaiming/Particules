/*
  ==============================================================================

	GranularEngine.cpp
	Created: 15 Feb 2023 1:44:09pm
	Author:  user

  ==============================================================================
*/

#include "GranularEngine.h"

GranularEngine::GranularEngine(StateParameters* stateParams) :
	stateParams(stateParams),
	scheduler(stateParams),
	fft(FFTSIZE_ORDER),
	phaseVocoderBuffer(stateParams->getNumChannels(), FFTSIZE),
	window(FFTSIZE, juce::dsp::WindowingFunction<float>::hann),
	circularBuffer(stateParams->getNumChannels(), FFTSIZE)
{
}

GranularEngine::~GranularEngine()
{
	stateParams = nullptr;
}

void GranularEngine::mixingProcess(AudioBlock wetBlock)
{
	//juce::Logger::outputDebugString("mix : " + (juce::String)stateParams->getMix());
	mixerProcessor.setWetMixProportion(stateParams->getMix());
	mixerProcessor.mixWetSamples(wetBlock);
}

void GranularEngine::gainProcess(juce::dsp::ProcessContextReplacing<float> context)
{
	gainProcessor.setGainLinear(stateParams->getGain());
	gainProcessor.process(context);
	//juce::Logger::outputDebugString("gain : " + (const juce::String)stateParams->getGain());
}

void GranularEngine::reverbProcess(juce::dsp::ProcessContextReplacing<float> context)
{
	reverbProcessor.process(context);
}

void GranularEngine::process(juce::AudioBuffer<float>& buffer, int numSamples)
{
	//int numChannel = buffer.getNumChannels();
	//grainBuffer.setSize(numChannel, numSamples);

	//juce::AudioBuffer<float> grainBuffer(buffer.getNumChannels(), buffer.getNumSamples());

	AudioBlock audioBlock(buffer);
	AudioBlock phaseVocoderBlock(phaseVocoderBuffer);
	//AudioBlock grainBlock(grainBuffer);
	//grainBlock.fill(0.0f);

	//AudioBlock grainBlock(grainBuffer);

	mixerProcessor.pushDrySamples(audioBlock);


	for (int sample = 0; sample < numSamples; ++sample) {
		scheduler.synthesize(&audioBlock, sample, numSamples);

		// 1. on met les samples dans le phasevocoder
		//phaseVocoderBuffer.addSample();

		// 2. quand le buffer est plein on multiplie par la window
		//window.multiplyWithWindowingTable(phaseVocoderBuffer.getWritePointer(channel), FFTSIZE);

		// 3. on applique le fft
		//fft.performFrequencyOnlyForwardTransform(phaseVocoderBuffer.getWritePointer(channel));

		// 4. on manipule le block

		// 5. on réassemble le contenu du block dans le buffer.
		// la taille du block n'est pas la même que la taille de déplacement de la région
		// On calcule tous les block de N samples mais on incrémente dans le buffer tous les M samples
		// Ce qui resulte à des block overlappé
	}

	//reverbProcess(audioBlock); // ok
	mixingProcess(audioBlock); // ok
	gainProcess(audioBlock); // ok


	//buffer.applyGainRamp(0, numSamples, stateParams->getPreviousGain(), stateParams->getGain());
	//stateParams->setPreviousGain(stateParams->getGain());
	//buffer.applyGain(stateParams->getGain());
}

// called by prepare to play method
void GranularEngine::init(int sampleRate, int numChannel, int samplePerBlocks)
{
	scheduler.init(numChannel);

	juce::dsp::ProcessSpec spec;

	spec.maximumBlockSize = samplePerBlocks;
	spec.numChannels = numChannel;
	spec.sampleRate = sampleRate;

	mixerProcessor.prepare(spec);
	mixerProcessor.setMixingRule(juce::dsp::DryWetMixingRule::balanced);

	gainProcessor.prepare(spec);
	gainProcessor.setRampDurationSeconds(0.02f);

	// to debug
	reverbProcessor.prepare(spec);
	params.roomSize = 1.0f;
	params.damping = 0.5f;
	params.wetLevel = 0.5f;
	params.dryLevel = 0.0f;
	params.width = 1.0f;
	params.freezeMode = false;
	reverbProcessor.setParameters(params);


}
