#include "Grain.h"
#include "utils/struct/SmoothedParameters.h"

namespace particules
{
    Grain::Grain()
        : durationSamples{0}, startPositionSamples{0}, speed{1.f}, sustainWidthSamples{0}, span{0}, fadeInSamples{0},
          fadeOutSamples{0}, elapsedSamples{0}, readPosition{0.f}, bufferNumSamples{0}, effectiveSpeed{1.f}, playback{1},
          payload{nullptr}, voiceID{-1}, envID{-1}, pitchRatio{1.0f}, gain{1.0f}
    {
        reset();
    }

    void Grain::reset()
    {
        durationSamples = 0;
        startPositionSamples = 0;
        sustainWidthSamples = 0;
        elapsedSamples = 0;
        fadeInSamples = 0;
        fadeOutSamples = 0;
        generation = 0;
        bufferNumSamples = 0;
        speed = 1.f;
        readPosition = 0.f;
        effectiveSpeed = 1.0f;
        playback = 1;
        gain = 1.0f;
        envID = -1;
        voiceID = -1;
    }

    void Grain::config(const ParameterSnapshot& ps, float normalizedPosMod, int voiceID, int envID, float pitchRatio, float gain)
    {
        this->elapsedSamples = 0;

        this->bufferNumSamples = ps.bufferNumSamples;
        this->durationSamples = ps.durationSamples;

        this->voiceID = voiceID;
        this->envID = envID;
        this->pitchRatio = pitchRatio;
        this->gain = gain;

        // position data
        const int positionModulationSamples = static_cast<int>(normalizedPosMod * ps.spanSamples);
        this->startPositionSamples = (ps.startPositionSamples + positionModulationSamples) % bufferNumSamples;
        this->readPosition = static_cast<float>(startPositionSamples);
        this->span = ps.spanSamples;

        this->speed = ps.speed;
        this->playback = ps.playback;
        this->effectiveSpeed = pitchRatio * playback;

        this->sustainWidthSamples = static_cast<int>(ps.sustainRatio * durationSamples);
        this->fadeInSamples = static_cast<int>(0.5f * (durationSamples - sustainWidthSamples));
        this->fadeOutSamples = fadeInSamples + sustainWidthSamples;
        this->fadeInSamples = std::max<int>(fadeInSamples, 1);
        this->fadeOutSamples = std::max<int>(fadeOutSamples, 1);
        this->invFadeInSamples = 1.f / static_cast<float>(fadeInSamples);

        // for smoothing value that updates during the lifecycle
        // envelope data
        //setEnvelopeData(static_cast<int>(ps.sustainRatio));
    }

    //void Grain::setEnvelopeData(const float sustainRatio) noexcept {}

    /*
    void Grain::updateParams(const SmoothedParameters& params) noexcept
    {
        speed = params.speed;
        //setEnvelopeData(params.sustainRatio);
    }
    */
    const float Grain::getReadPosition() const noexcept { return readPosition; }

    void Grain::nextReadPosition() noexcept
    {
        elapsedSamples++;
        readPosition += effectiveSpeed;

        while(readPosition >= static_cast<float>(bufferNumSamples))
            readPosition -= static_cast<float>(bufferNumSamples);

        // effective speed can be negative
        while(readPosition < 0.0f)
            readPosition += static_cast<float>(bufferNumSamples);
    }

    // EnvelopeLookUpTable
    // fadeIn  : 0 - 0.5f
    // sustain : 0.5f
    // fadeOut : 0.5f - 1.f
    const float Grain::getPhase() const noexcept
    {
        if(durationSamples <= 0)
            return 0.f;

        const float elapsed = static_cast<float>(elapsedSamples);

        if(elapsed < static_cast<float>(fadeInSamples))
            return 0.5f * (elapsed * invFadeInSamples);
        else if(elapsed < static_cast<float>(fadeOutSamples))
            return 0.5f;
        else
        {
            const float stepsIntoRelease = elapsed - static_cast<float>(fadeOutSamples);
            const float releaseProgress = stepsIntoRelease * invFadeInSamples;
            return 0.5f + 0.5f * releaseProgress;
        }
    }
}