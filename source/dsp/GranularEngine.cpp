/*
  ==============================================================================

	GranularEngine.cpp
	Created: 15 Feb 2023 1:44:09pm
	Author:  user

  ==============================================================================
*/

#include "GranularEngine.h"
#include "../framework/ParameterView.h"

GranularEngine::GranularEngine(ParameterView& sp) : paramsView{sp}, scheduler{}, voiceManager{grainPool}, grainPool{} {}

void GranularEngine::process(juce::AudioBuffer<float>& bufferOut, int bufferSize)
{
    // pour 1024 buffer size en 48kHz on a une fenetre de 21ms par appelle de compute.
    // si density = 500g/s (1g chaque 0.002s) alors on a interOnSet = 48000/500 = 96 sample.
    // 1024/96 = 10.66 grains par appel


    const ParameterSnapshot snapshot = paramsView.getSnapshot();
    const SampleSource* source = paramsView.getSampleSource().get();

    if(!snapshot.isValid())
    {
        bufferOut.clear();
        return;
    }

    AudioBlock outputBlock(bufferOut);

    mixerProcessor.pushDrySamples(outputBlock);

    scheduler.process(
        bufferSize, source->sampleRate, snapshot.density,
        [this](int offset, const ParameterSnapshot& snapshot) { voiceManager.spawn(offset, snapshot); }, snapshot);

    voiceManager.process(outputBlock, bufferSize, source);

    //reverbProcess(audioBlock);
    mixingProcess(outputBlock);
    gainProcess(outputBlock);
}

// called by prepare to play method
void GranularEngine::init(int sampleRate, int numChannel, int samplePerBlocks)
{
    //scheduler.reset();
    //juce::Logger::outputDebugString("Granuler Engine init numChannel is : " + juce::String(numChannel));
    juce::dsp::ProcessSpec spec;

    spec.maximumBlockSize = samplePerBlocks;
    spec.numChannels = numChannel;
    spec.sampleRate = sampleRate;

    mixerProcessor.prepare(spec);
    mixerProcessor.setMixingRule(juce::dsp::DryWetMixingRule::balanced);

    gainProcessor.prepare(spec);
    gainProcessor.setRampDurationSeconds(0.02f);

    // TODO pre allouer les grains dans le grain pool

    // to debug
    //reverbProcessor.prepare(spec);
    //params.roomSize = 1.0f;
    //params.damping = 0.5f;
    //params.wetLevel = 0.5f;
    //params.dryLevel = 0.0f;
    //params.width = 1.0f;
    //params.freezeMode = false;
    //reverbProcessor.setParameters(params);
}

void GranularEngine::mixingProcess(AudioBlock wetBlock)
{
    mixerProcessor.setWetMixProportion(paramsView.getMix());
    mixerProcessor.mixWetSamples(wetBlock);
}

void GranularEngine::gainProcess(juce::dsp::ProcessContextReplacing<float> context)
{
    gainProcessor.setGainLinear(paramsView.getGain());
    gainProcessor.process(context);
}

void GranularEngine::reverbProcess(juce::dsp::ProcessContextReplacing<float> context) { reverbProcessor.process(context); }