#include "GranularEngine.h"
#include "../framework/bridge/AudioState.h"
#include "../utils/struct/ParameterSnapshot.h"
#include "GranularEngine.h"

namespace particules
{
    GranularEngine::GranularEngine(LockFreeDoubleBuffer<VisualSnapshot>& vb, AudioState& as)
        : scheduler{}, grainProcessor{pool, posMod, envLut}, pool{}, posMod{}, refreshRate{gui::refreshRate},
          sampleAccumulator{0}, threshold{0}, smoothedParams{}, audioState{as}, visualBuffer{vb}
    {
        spawnCallback = [this](const ParameterSnapshot& ps) { grainProcessor.spawn(ps); }; // to avoid creating a new lambda every sampleBlock
    }

    // pour 1024 buffer size en 48kHz on a une fenetre de 21ms par appelle de compute.
    // si Emission = 500g/s (1g chaque 0.002s) alors on a interOnSet = 48000/500 = 96 sample.
    // 1024/96 = 10.66 grains par appel
    void GranularEngine::process(
        AudioBuffer& bufferOut, int bufferSize, float* const* outputPtrs, int outputNumChannels, const ParameterSnapshot& ps)
    {
        // *** security *** //

        auto bufferGuard = inputBufferPtr.load();
        if(!bufferGuard)
            return;

        // *** parameters *** //

        const AudioBuffer* inputBuffer = bufferGuard.get();

        AudioBlock outputBlock(bufferOut);

        const float* const* inputPtrs = inputBuffer->getArrayOfReadPointers();
        const int inputNumsChannels = inputBuffer->getNumChannels();
        const int inputNumSamples = inputBuffer->getNumSamples();

        jassert(inputNumsChannels == outputNumChannels);

        setTargetSmoothedValue(ps);
        posMod.setParameters(ps.traversalMode, ps.traversalFreq);
        scheduler.setEmission(ps.emission);

        // *** dsp *** //

        for(int currentSample = 0; currentSample < bufferSize; currentSample++)
        {
            updateSmoothedParameters();
            scheduler.tick(spawnCallback, ps);
            grainProcessor.render(currentSample, outputNumChannels, outputPtrs, inputPtrs, smoothedParams);

            //const float env = adsr.getNextSample();
            //for(int ch = 0; ch < outputNumChannels; ++ch)
            //outputPtrs[ch][currentSample] *= env;
        }

        posMod.advanceBlock(bufferSize);

        gainProcess(outputBlock, ps.linearGain);

        // *** UI *** //

        // write at 30hz speed
        sampleAccumulator += bufferSize;
        while(sampleAccumulator >= threshold)
        {
            sampleAccumulator -= threshold;
            writeVisualSnapshot();
            audioState.setNumActiveGrains(pool.getNumActiveGrains());
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

    void GranularEngine::writeVisualSnapshot() noexcept
    {
        VisualSnapshot& snap = visualBuffer.beginWriteBuffer();
        grainProcessor.writeVisualSnapshot(snap);
        visualBuffer.endWriteBuffer();
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

    void GranularEngine::setTargetSmoothedValue(const ParameterSnapshot& ps) noexcept
    {
        if(speedSmooth.getTargetValue() != ps.speed)
            speedSmooth.setTargetValue(ps.speed);

        //if(sustainRatioSmooth.getTargetValue() != ps.sustainRatio)
        //    sustainRatioSmooth.setTargetValue(ps.sustainRatio);
    }
}