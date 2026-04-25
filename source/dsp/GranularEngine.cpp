#include "GranularEngine.h"
#include "framework/bridge/FromAudio.h"
#include "framework/state/AudioState.h"
#include "utils/struct/AudioPayload.h"
#include "utils/struct/ParameterSnapshot.h"

namespace particules
{
    GranularEngine::GranularEngine(FromAudio& fa)
        : grainProcessor{pool, posMod, envLut, voiceManager},
          voiceManager{[this](const ParameterSnapshot& ps, AudioPayload* payload, int indexVoice, float pitchRatio,
                           float velocity) -> void { grainProcessor.spawn(ps, payload, indexVoice, pitchRatio, velocity); }},
          pool{}, posMod{}, refreshRate{gui::refreshRate}, sampleAccumulator{0}, threshold{0}, smoothedParams{}, faudio{fa}
    {
        //spawnCallback = [this](const ParameterSnapshot& ps, AudioPayload* payload) {
        //    grainProcessor.spawn(ps, payload);
        //}; // to avoid creating a new lambda every sampleBlock
    }

    // pour 1024 buffer size en 48kHz on a une fenetre de 21ms par appelle de compute.
    // si Emission = 500g/s (1g chaque 0.002s) alors on a interOnSet = 48000/500 = 96 sample.
    // 1024/96 = 10.66 grains par appel
    void GranularEngine::process(AudioBuffer& outputBuffer, juce::MidiBuffer& midiBuffer, AudioPayload* payload, int bufferSize,
        float* const* outputPtrs, int outputNumChannels, const ParameterSnapshot& ps)
    {
        if(!payload)
            return;
        AudioBuffer* inputBuffer = payload->buffer.get();

        // midi setup
        juce::MidiBuffer::Iterator midiIterator(midiBuffer);
        juce::MidiMessage midiMsg;
        int midiSamplePosition = 0;
        bool hasMidiEvent = midiIterator.getNextEvent(midiMsg, midiSamplePosition);

        // parameters
        AudioBlock outputBlock(outputBuffer);

        //const float* const* inputPtrs = inputBuffer->getArrayOfReadPointers();
        //const int inputNumSamples = inputBuffer->getNumSamples();
        const int inputNumsChannels = inputBuffer->getNumChannels();

        // security
        jassert(inputNumsChannels == outputNumChannels);

        //setTargetSmoothedValue(ps);
        posMod.setParameters(ps.traversalMode, ps.traversalFreq);
        voiceManager.setParameters(ps.attack, ps.decay, ps.sustain, ps.release);

        // main loop
        for(int currentSample = 0; currentSample < bufferSize; currentSample++)
        {
            //updateSmoothedParameters();
            while(midiSamplePosition == currentSample && hasMidiEvent)
            {
                if(midiMsg.isNoteOn() && midiMsg.getVelocity() > 0)
                {
                    voiceManager.noteOn(midiMsg.getNoteNumber(), midiMsg.getFloatVelocity());
                }
                else if(midiMsg.isNoteOff() || midiMsg.isNoteOn(0))
                {
                    voiceManager.noteOff(midiMsg.getNoteNumber());
                }
                else if(midiMsg.isAllNotesOff())
                {
                    voiceManager.allNotesOff();
                }

                hasMidiEvent = midiIterator.getNextEvent(midiMsg, midiSamplePosition);
            }

            voiceManager.process(currentSample, ps, payload /*, smoothedParams*/);
            grainProcessor.process(currentSample, outputNumChannels, outputPtrs /*, smoothedParams*/);
        }

        posMod.advanceBlock(bufferSize);
        gainProcess(outputBlock, ps.linearGain);

        // write at 30hz speed
        sampleAccumulator += bufferSize;
        while(sampleAccumulator >= threshold)
        {
            sampleAccumulator -= threshold;
            VisualSnapshot& snap = faudio.beginWriteVisualSnapshot();
            grainProcessor.writeVisualSnapshot(snap);
            faudio.endWriteVisualSnapshot();
            faudio.setNumActiveGrains(pool.getNumActiveGrains());
        }
    }

    // called by pluginprocessor.prepareToPlay()
    void GranularEngine::init(const double sampleRate, const int numChannel, const int samplePerBlocks)
    {
        threshold = static_cast<int>(sampleRate / refreshRate);
        posMod.setSampleRate(sampleRate);
        voiceManager.setSampleRate(sampleRate);

        juce::dsp::ProcessSpec spec;

        spec.maximumBlockSize = samplePerBlocks;
        spec.numChannels = numChannel;
        spec.sampleRate = sampleRate;

        gainProcessor.prepare(spec);
        gainProcessor.setRampDurationSeconds(0.02f);

        //adsr.setSampleRate(sampleRate);

        // temporary : later will be apvts
        //adsrParams.attack = 0.5f;
        //adsrParams.decay = 0.f;
        //adsrParams.sustain = 1.f;
        //adsrParams.release = 1.f;
        //adsr.setParameters(adsrParams);

        // init smooth parameters
        /*
        speedSmooth.reset(sampleRate, 0.02);
        attackSmooth.reset(sampleRate, 0.02);
        decaySmooth.reset(sampleRate, 0.02);
        sustainSmooth.reset(sampleRate, 0.02);
        releaseSmooth.reset(sampleRate, 0.02);
        */
        //sustainRatioSmooth.reset(sampleRate, 0.01);

        //scheduler.init(sampleRate);
    }

    void GranularEngine::clear() noexcept { grainProcessor.reset(); }

    void GranularEngine::gainProcess(juce::dsp::ProcessContextReplacing<float> context, const float gainLin)
    {
        gainProcessor.setGainLinear(gainLin);
        gainProcessor.process(context);
    }

    void GranularEngine::updateSmoothedParameters() noexcept
    {
        /*
        smoothedParams.speed = speedSmooth.getNextValue();
        smoothedParams.attack = attackSmooth.getNextValue();
        smoothedParams.decay = decaySmooth.getNextValue();
        smoothedParams.sustain = sustainSmooth.getNextValue();
        smoothedParams.release = releaseSmooth.getNextValue();
        */
        //smoothedParams.sustainRatio = sustainRatioSmooth.getNextValue();
    }

    void GranularEngine::setTargetSmoothedValue(const ParameterSnapshot& ps) noexcept
    {
        /*
        if(speedSmooth.getTargetValue() != ps.speed)
            speedSmooth.setTargetValue(ps.speed);
        if(attackSmooth.getTargetValue() != ps.attack)
            attackSmooth.setTargetValue(ps.attack);
        if(decaySmooth.getTargetValue() != ps.decay)
            decaySmooth.setTargetValue(ps.decay);
        if(sustainSmooth.getTargetValue() != ps.sustain)
            sustainSmooth.setTargetValue(ps.sustain);
        if(releaseSmooth.getTargetValue() != ps.release)
            releaseSmooth.setTargetValue(ps.release);
        */

        //if(sustainRatioSmooth.getTargetValue() != ps.sustainRatio)
        //    sustainRatioSmooth.setTargetValue(ps.sustainRatio);
    }
}