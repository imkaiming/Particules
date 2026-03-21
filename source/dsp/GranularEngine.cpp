#include "GranularEngine.h"
#include "../framework/ParameterView.h"

GranularEngine::GranularEngine(GrainVisualBuffer& vb)
    : scheduler{}, voiceManager{pool, posMod, envLut, vb}, pool{}, posMod{}, refreshRate{60.f}, accumulator{0}, threshold{0},
      smoothedParams{}
{
    spawnCallback = [this](const ParameterSnapshot& s) { voiceManager.spawn(s); };
}

// pour 1024 buffer size en 48kHz on a une fenetre de 21ms par appelle de compute.
// si Emission = 500g/s (1g chaque 0.002s) alors on a interOnSet = 48000/500 = 96 sample.
// 1024/96 = 10.66 grains par appel
void GranularEngine::process(
    AudioBuffer& bufferOut, int bufferSize, float* const* outputPtrs, int outputNumChannels, const ParameterSnapshot snapshot)
{
    auto bufferGuard = inputBuffer.load();
    //std::shared_ptr<const AudioBuffer> bufferGuard = inputBuffer.load();
    if(!bufferGuard)
        return;

    const AudioBuffer* inputBuffer = bufferGuard.get();

    AudioBlock outputBlock(bufferOut);

    const float* const* inputPtrs = inputBuffer->getArrayOfReadPointers();
    const int inputNumsChannels = inputBuffer->getNumChannels();
    const int inputNumSamples = inputBuffer->getNumSamples();

    jassert(inputNumsChannels == outputNumChannels);

    setTargetSmoothedValue(snapshot);
    posMod.setParameters(snapshot.traversalMode, snapshot.traversalFreq);
    scheduler.setEmission(snapshot.emission);

    for(int currentSample = 0; currentSample < bufferSize; currentSample++)
    {
        updateSmoothedParameters();
        scheduler.tick(spawnCallback, snapshot);
        voiceManager.render(currentSample, outputNumChannels, outputPtrs, inputPtrs, inputNumSamples, smoothedParams);

        const float env = adsr.getNextSample();
        for(int ch = 0; ch < outputNumChannels; ++ch)
            outputPtrs[ch][currentSample] *= env;
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

    adsr.setSampleRate(sampleRate);

    // temporary : later will be apvts
    adsrParams.attack = 0.5f;
    adsrParams.decay = 0.f;
    adsrParams.sustain = 1.f;
    adsrParams.release = 1.f;
    adsr.setParameters(adsrParams);

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
