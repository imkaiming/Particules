#include "Grain.h"
#include "utils/struct/SmoothedParameters.h"

namespace particules
{
    Grain::Grain()
        : durationSamples{0}, startPositionSamples{0}, speed{1.f}, sustainWidthSamples{0}, span{0}, fadeInSamples{0},
          fadeOutSamples{0}, elapsedSamples{0}, readPosition{0.f}, inputNumSamples{0}, effectiveSpeed{1.f}, playback{1}
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
        inputNumSamples = 0;
        speed = 1.f;
        readPosition = 0.f;
        effectiveSpeed = 1.0f;
        playback = 1;
    }

    void Grain::config(const ParameterSnapshot& ps, float normalizedPosMod)
    {
        elapsedSamples = 0;

        inputNumSamples = ps.inputNumSamples;
        durationSamples = ps.durationSamples;

        // position data
        const int positionModulationSamples = static_cast<int>(normalizedPosMod * ps.spanSamples);
        startPositionSamples = (ps.startPositionSamples + positionModulationSamples) % inputNumSamples;
        readPosition = static_cast<float>(startPositionSamples);
        span = ps.spanSamples;

        speed = ps.speed;
        playback = ps.playback;
        effectiveSpeed = speed * playback;

        // envelope data
        //setEnvelopeData(static_cast<int>(ps.sustainRatio));

        //linearGain = ps.linearGain;
        sustainWidthSamples = static_cast<int>(ps.sustainRatio * durationSamples);
        fadeInSamples = static_cast<int>(0.5f * (durationSamples - sustainWidthSamples));
        fadeOutSamples = fadeInSamples + sustainWidthSamples;
        fadeInSamples = std::max<int>(fadeInSamples, 1);
        fadeOutSamples = std::max<int>(fadeOutSamples, 1);
        invFadeInSamples = 1.f / static_cast<float>(fadeInSamples);
    }

    //void Grain::setEnvelopeData(const float sustainRatio) noexcept {}

    void Grain::updateParams(const SmoothedParameters& params) noexcept
    {
        speed = params.speed;
        //setEnvelopeData(params.sustainRatio);
    }
    const float Grain::getReadPosition() const noexcept { return readPosition; }

    void Grain::nextReadPosition() noexcept
    {
        elapsedSamples++;
        readPosition += effectiveSpeed;

        while(readPosition >= static_cast<float>(inputNumSamples))
            readPosition -= static_cast<float>(inputNumSamples);

        // effective speed can be negative
        while(readPosition < 0.0f)
            readPosition += static_cast<float>(inputNumSamples);
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