/*
  ==============================================================================

	GranularEngine.cpp
	Created: 15 Feb 2023 1:44:09pm
	Author:  user

  ==============================================================================
*/

#include "GranularEngine.h"
#include "../framework/ParameterView.h"

GranularEngine::GranularEngine(GrainVisualBuffer& vb)
    : scheduler{}, voiceManager{pool, posMod, envLut, vb}, pool{}, posMod{}, refreshRate{60.f}, accumulator{0}, threshold{0}
{
}

void GranularEngine::process(juce::AudioBuffer<float>& bufferOut, int bufferSize, const ParameterSnapshot snapshot)
{
    // pour 1024 buffer size en 48kHz on a une fenetre de 21ms par appelle de compute.
    // si Emission = 500g/s (1g chaque 0.002s) alors on a interOnSet = 48000/500 = 96 sample.
    // 1024/96 = 10.66 grains par appel

    std::shared_ptr<const AudioBuffer> bufferGuard = inputBuffer.load();
    if(!bufferGuard)
        return;

    const AudioBuffer* inputBuffer = bufferGuard.get();

    AudioBlock outputBlock(bufferOut);

    posMod.setParameters(snapshot.traversalMode, snapshot.traversalFreq);

    // initiating blocks before looping if needed : scheduler.setBufferSize(bufferSize) ???

    // TODO : up the for loop sample here and make the scheduler processing sample per sample and not block per block
    // its going to be easier for the cache and for parameter smoothing also.
    speedSmooth.setCurrentAndTargetValue(snapshot.speed);
    sustainRatioSmooth.setCurrentAndTargetValue(snapshot.sustainRatio);

    scheduler.setEmission(snapshot.emission);

    const int numChannels = static_cast<int>(outputBlock.getNumChannels());
    //const int inputNumChannels = inputSource->getNumChannels();
    //const int inputNumSamples = inputSource->getNumSamples();

    for(int currentSample = 0; currentSample < bufferSize; currentSample++)
    {
        /* smoothing parameters here */
        scheduler.tick(currentSample, [this](int i, const ParameterSnapshot& s) { voiceManager.spawn(i, s); }, snapshot);
        voiceManager.render(currentSample, numChannels, outputBlock, inputBuffer);
    }

    //scheduler.process(
    //    bufferSize, snapshot.sampleRate, snapshot.emission,
    //    [this](int offset, const ParameterSnapshot& snapshot) { voiceManager.spawn(offset, snapshot); }, snapshot);
    //voiceManager.process(outputBlock, bufferSize, inputBuffer);

    posMod.advanceBlock(bufferSize);

    gainProcess(outputBlock, snapshot.linearGain);

    accumulator += bufferSize;
    while(accumulator >= threshold)
    {
        accumulator -= threshold;
        voiceManager.writeVisualSnapshot();
    }
}

// called by pluginprocessor.prepareToPlay()
void GranularEngine::init(double sampleRate, int numChannel, int samplePerBlocks)
{
    threshold = static_cast<int>(sampleRate / refreshRate);
    posMod.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;

    spec.maximumBlockSize = samplePerBlocks;
    spec.numChannels = numChannel;
    spec.sampleRate = sampleRate;

    gainProcessor.prepare(spec);
    gainProcessor.setRampDurationSeconds(0.02f);

    speedSmooth.reset(sampleRate, 0.02);
    sustainRatioSmooth.reset(sampleRate, 0.01);

    scheduler.init(sampleRate);
}

void GranularEngine::gainProcess(juce::dsp::ProcessContextReplacing<float> context, const float gainLin)
{
    gainProcessor.setGainLinear(gainLin);
    gainProcessor.process(context);
}
