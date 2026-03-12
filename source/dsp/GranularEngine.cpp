/*
  ==============================================================================

	GranularEngine.cpp
	Created: 15 Feb 2023 1:44:09pm
	Author:  user

  ==============================================================================
*/

#include "GranularEngine.h"
#include "../framework/ParameterView.h"

GranularEngine::GranularEngine(ParameterView& sp, GrainVisualBuffer& vb)
    : paramsView{sp}, scheduler{}, voiceManager{pool, posMod, envLut, vb}, pool{}, posMod{sp.getSampleRate()}, refreshRate{60.f},
      accumulator{0}, threshold{0}
{
}

void GranularEngine::process(juce::AudioBuffer<float>& bufferOut, int bufferSize)
{
    // pour 1024 buffer size en 48kHz on a une fenetre de 21ms par appelle de compute.
    // si Emission = 500g/s (1g chaque 0.002s) alors on a interOnSet = 48000/500 = 96 sample.
    // 1024/96 = 10.66 grains par appel

    const ParameterSnapshot snapshot = paramsView.getSnapshot();
    const AudioBuffer* inputSource = paramsView.getAudioSource().get();

    if(!snapshot.isValid())
    {
        bufferOut.clear();
        return;
    }

    AudioBlock outputBlock(bufferOut);

    posMod.setParameters(snapshot.traversalMode, snapshot.traversalFreq);

    scheduler.process(
        bufferSize, snapshot.sampleRate, snapshot.emission,
        [this](int offset, const ParameterSnapshot& snapshot) { voiceManager.spawn(offset, snapshot); }, snapshot);

    voiceManager.process(outputBlock, bufferSize, inputSource);

    posMod.advanceBlock(bufferSize);

    gainProcess(outputBlock);

    accumulator += bufferSize;
    while(accumulator >= threshold)
    {
        accumulator -= threshold;
        voiceManager.writeVisualSnapshot();
    }
}

// called by prepare to play method
void GranularEngine::init(double sampleRate, int numChannel, int samplePerBlocks)
{
    //scheduler.reset();
    //juce::Logger::outputDebugString("Granuler Engine init numChannel is : " + juce::String(numChannel));
    threshold = static_cast<int>(sampleRate / refreshRate);
    posMod.setSampleRate(paramsView.getSampleRate());

    juce::dsp::ProcessSpec spec;

    spec.maximumBlockSize = samplePerBlocks;
    spec.numChannels = numChannel;
    spec.sampleRate = sampleRate;

    //mixerProcessor.prepare(spec);
    //mixerProcessor.setMixingRule(juce::dsp::DryWetMixingRule::balanced);

    gainProcessor.prepare(spec);
    gainProcessor.setRampDurationSeconds(0.02f);

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

//void GranularEngine::mixingProcess(AudioBlock wetBlock)
//{
//    mixerProcessor.setWetMixProportion(paramsView.getMix());
//    mixerProcessor.mixWetSamples(wetBlock);
//}

void GranularEngine::gainProcess(juce::dsp::ProcessContextReplacing<float> context)
{
    gainProcessor.setGainLinear(paramsView.getLinearGain());
    gainProcessor.process(context);
}

//void GranularEngine::reverbProcess(juce::dsp::ProcessContextReplacing<float> context) { reverbProcessor.process(context); }