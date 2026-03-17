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
    : scheduler{}, voiceManager{pool, posMod, envLut, vb}, pool{}, posMod{}, refreshRate{60.f}, accumulator{0}, threshold{0},
      smoothedParams{}
{
    spawnCallback = [this](int i, const ParameterSnapshot& s) { voiceManager.spawn(i, s); };
}

// pour 1024 buffer size en 48kHz on a une fenetre de 21ms par appelle de compute.
// si Emission = 500g/s (1g chaque 0.002s) alors on a interOnSet = 48000/500 = 96 sample.
// 1024/96 = 10.66 grains par appel
void GranularEngine::process(juce::AudioBuffer<float>& bufferOut, int bufferSize, const ParameterSnapshot snapshot)
{
    auto bufferGuard = inputBuffer.load();
    //std::shared_ptr<const AudioBuffer> bufferGuard = inputBuffer.load();
    if(!bufferGuard)
        return;

    const AudioBuffer* inputBuffer = bufferGuard.get();

    AudioBlock outputBlock(bufferOut);

    // configuration
    setTargetSmoothedValue(snapshot);
    posMod.setParameters(snapshot.traversalMode, snapshot.traversalFreq);
    scheduler.setEmission(snapshot.emission);

    const int numChannels = static_cast<int>(outputBlock.getNumChannels());
    //const int inputNumChannels = inputSource->getNumChannels();
    //const int inputNumSamples = inputSource->getNumSamples();

    // processing
    for(int currentSample = 0; currentSample < bufferSize; currentSample++)
    {
        updateSmoothedParameters();
        scheduler.tick(currentSample, spawnCallback, snapshot);
        voiceManager.render(currentSample, numChannels, outputBlock, inputBuffer, smoothedParams);
    }

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
void GranularEngine::init(const double sampleRate, const int numChannel, const int samplePerBlocks)
{
    threshold = static_cast<int>(sampleRate / refreshRate);
    posMod.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;

    spec.maximumBlockSize = samplePerBlocks;
    spec.numChannels = numChannel;
    spec.sampleRate = sampleRate;

    gainProcessor.prepare(spec);
    gainProcessor.setRampDurationSeconds(0.02f);

    // init smooth parameters
    speedSmooth.reset(sampleRate, 0.02);
    //sustainRatioSmooth.reset(sampleRate, 0.01);

    scheduler.init(sampleRate);
}

void GranularEngine::gainProcess(juce::dsp::ProcessContextReplacing<float> context, const float gainLin)
{
    gainProcessor.setGainLinear(gainLin);
    gainProcessor.process(context);
}

void GranularEngine::updateSmoothedParameters() noexcept
{
    smoothedParams.speed = speedSmooth.getNextValue();
    //smoothedParams.sustainRatio = sustainRatioSmooth.getNextValue();
}

void GranularEngine::setTargetSmoothedValue(const ParameterSnapshot& snapshot) noexcept
{
    if(speedSmooth.getTargetValue() != snapshot.speed)
        speedSmooth.setTargetValue(snapshot.speed);

    //if(sustainRatioSmooth.getTargetValue() != snapshot.sustainRatio)
    //    sustainRatioSmooth.setTargetValue(snapshot.sustainRatio);
}
