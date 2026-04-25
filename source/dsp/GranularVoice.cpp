#include "dsp/GranularVoice.h"

#include "utils/struct/ParameterSnapshot.h"

namespace particules
{
    GranularVoice::GranularVoice() : sampleRate{0.0}, note{-1}, velocity{0}, pitch{1.0f}, noteOnTime{0}
    {
        adsr.setParameters({0.1f, 1.f, 1.f, 1.f});
    }

    void GranularVoice::setSampleRate(double sampleRate) noexcept
    {
        this->sampleRate = sampleRate;
        adsr.setSampleRate(sampleRate);
        scheduler.setSampleRate(sampleRate);
    }

    void GranularVoice::noteOn(int midiNoteNumber, float velocity, float pitch, uint32_t noteOnTime)
    {
        this->note = midiNoteNumber;
        this->velocity = velocity;
        this->pitch = pitch;
        this->noteOnTime = noteOnTime;

        adsr.noteOn();
    }

    void GranularVoice::noteOff() { adsr.noteOff(); }

    void GranularVoice::tick(const ParameterSnapshot& ps, AudioPayload* payload, int voiceID)
    {
        if(!isBusy())
            return;

        float currentAdsrLevel = adsr.getNextSample();
        float gain = currentAdsrLevel * velocity;
        scheduler.tick(spawnGrainCallback, ps, payload, voiceID, pitch, gain);
    }

    void GranularVoice::setADSR(float attack, float decay, float sustain, float release) noexcept
    {
        adsr.setParameters({attack, decay, sustain, release});
    }
}