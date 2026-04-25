#pragma once
#include "framework/core/Core.h"
#include "utils/struct/ParameterSnapshot.h"
#include "utils/struct/SmoothedParameters.h"

// Grain is the unit that read samples the buffer
// It have 2 main structure time and amplitude
// a grain have a fade in then a sustain then a fade out
namespace particules
{
    struct AudioPayload;
    class Grain
    {
    public:
        Grain();
        ~Grain() = default;

        void reset();
        void config(const ParameterSnapshot&, float, int, int, float, float);

        const float getReadPosition() const noexcept;
        void nextReadPosition() noexcept;
        //void updateParams(const SmoothedParameters&) noexcept;
        void setActive(bool b) noexcept { active = b; }
        void incrementGen() noexcept { generation++; }

        uint16_t getGeneration() const noexcept { return generation; }
        bool isExhausted() const noexcept { return elapsedSamples >= durationSamples; }
        const float getPhase() const noexcept;
        bool getActive() const noexcept { return active; }
        int getVoiceID() const noexcept { return voiceID; }
        const float getGain() const noexcept { return gain; };
        const int getEnvID() const noexcept { return envID; };

        AudioPayload* payload;

    private:
        // Time related parameters
        int elapsedSamples; // le compteur interne du grain
        int durationSamples; // définie la durée en nombre de sample
        int startPositionSamples; // définie la position en sample dans le buffer
        int span; // définie la position maximale qu'un grain peut atteindre dans le buffer
        //int delaySamples; // act like a little delay to sync scheduler.process() and voiceManager.process()
        int sustainWidthSamples; // définie la taille des rampes d'amplitude en entré et en sortie du grain
        int fadeInSamples; // 0 to fadeIn
        int fadeOutSamples; // fadeOut to durationSamples
        int inputNumSamples;
        float playback;
        //int inputNumChannels;

        float speed; // change the pitch and accelerate the lecture
        float readPosition;
        float invFadeInSamples;
        float effectiveSpeed;
        //float linearGain;

        int voiceID; // index 
        int envID; 
        float pitchRatio;
        float gain;

        // lifecycle
        uint16_t generation = 0;
        bool active = false;
    };
}