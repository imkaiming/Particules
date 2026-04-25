#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include "dsp/Scheduler.h"
#include "framework/core/PluginTypes.h"

namespace particules
{
    struct ParameterSnapshot;
    struct AudioPayload;
    class GranularVoice
    {
    public:
        GranularVoice();
        ~GranularVoice() = default;

        void setCallback(SpawnGrainCallback& c) { spawnGrainCallback = c; }

        void tick(const ParameterSnapshot& ps, AudioPayload* payload, int indexVoice);

        void noteOn(int midiNoteNumber, float velocity, float pitchRatio, uint32_t sampleCounter);
        void noteOff();
        void allNotesOff();

        bool isBusy() const noexcept { return adsr.isActive(); }
        int getCurrentNote() const noexcept { return note; }
        uint32_t getNoteOnTime() const noexcept { return noteOnTime; }
        float getPitchRatio() const noexcept { return pitch; }
        float getVelocity() const noexcept { return velocity; }
        float getGain() noexcept { return adsr.getNextSample() * velocity; } // ADSR * Velocity

        void setADSR(float attack, float decay, float sustain, float release) noexcept;
        void setEmission(float e) noexcept { scheduler.setEmission(e); }
        void setSampleRate(double sampleRate) noexcept;

    private:
        juce::ADSR adsr;
        Scheduler scheduler;
        SpawnGrainCallback spawnGrainCallback;

        int note;
        float velocity;
        float pitch;
        uint32_t noteOnTime;
        double sampleRate;
    };
}