#pragma once

#include "../../framework/PluginParams.h"
#include "Map.h"
#include "MathConstants.h"

namespace particules
{
    namespace dsp
    {
        static constexpr int SIZE = particules::params::maxLutSize;

        static void initHann(std::span<float> table) noexcept
        {
            const float p = twoPi / static_cast<float>(SIZE - 1);
            for(size_t i = 0; i < SIZE; i++)
                table[i] = 0.5f - 0.5f * std::cos(p * static_cast<float>(i));
        }

        static void initGaussian(std::span<float> table) noexcept
        {
            const float inv = 1.f / (SIZE - 1);
            const float sigma = 0.18f;
            const float invSigma = 1.f / sigma;

            for(size_t i = 0; i < SIZE; ++i)
            {
                const float x = (2.f * (i * inv) - 1.f) * invSigma;
                table[i] = std::exp(-0.5f * x * x);
            }

            table[0] = 0.f;
            table[SIZE - 1] = 0.f;
        }

        static void initLinear(std::span<float> table) noexcept
        {
            const float center = 0.5f * static_cast<float>(SIZE - 1.f);
            const float invCenter = 1.f / center;

            for(size_t i = 0; i < SIZE; ++i)
            {
                const float x = std::abs(i - center) * invCenter;
                table[i] = 1.f - x;
            }

            table[0] = 0.f;
            table[SIZE - 1] = 0.f;
        }

        static void initExp(std::span<float> table) noexcept
        {
            const float inv = 1.f / (SIZE - 1);
            const float k = 6.f;

            for(size_t i = 0; i < SIZE; ++i)
            {
                const float x = std::abs(2.f * (i * inv) - 1.f);
                table[i] = std::exp(-k * x);
            }

            table[0] = 0.f;
            table[SIZE - 1] = 0.f;
        }

        static void initSqrt(std::span<float> table) noexcept
        {
            const float inv = 1.f / static_cast<float>(table.size() - 1);

            for(size_t i = 0; i < table.size(); ++i)
            {
                const float x = static_cast<float>(i) * inv;
                const float distanceAuCentre = std::abs(2.f * x - 1.f);
                table[i] = std::sqrt(1.f - distanceAuCentre);
            }

            table[0] = 0.f;
            table[table.size() - 1] = 0.f;
        }

    }

    namespace gui
    {
        inline float evaluateWindow(EnvelopeMode mode, float phase) noexcept
        {
            phase = (phase < 0.0f) ? 0.0f : ((phase > 1.0f) ? 1.0f : phase);

            switch(mode)
            {
                case EnvelopeMode::Hann:
                    return 0.5f - 0.5f * std::cos(twoPi * phase);

                case EnvelopeMode::Linear:
                    return 1.0f - std::abs(2.0f * phase - 1.0f);

                case EnvelopeMode::Sqrt:
                    return std::sqrt(1.0f - std::abs(2.0f * phase - 1.0f));

                case EnvelopeMode::Gaussian:
                {
                    if(phase <= 0.0f || phase >= 1.0f)
                        return 0.0f;
                    const float x = (2.0f * phase - 1.0f) / 0.18f;
                    return std::exp(-0.5f * x * x);
                }

                case EnvelopeMode::Exp:
                {
                    if(phase <= 0.0f || phase >= 1.0f)
                        return 0.0f;
                    const float x = std::abs(2.0f * phase - 1.0f);
                    return std::exp(-6.0f * x);
                }

                default:
                    return 0.0f;
            }
        }

        inline float evaluateEnvelopeWithPlateau(EnvelopeMode mode, float phase, float sustainRatio) noexcept
        {
            if(sustainRatio <= 0.0f)
                return evaluateWindow(mode, phase);

            const float slopeTime = (1.0f - sustainRatio) * 0.5f;

            if(phase < slopeTime)
            {
                float mappedPhase = (phase / slopeTime) * 0.5f;
                return evaluateWindow(mode, mappedPhase);
            }
            else if(phase > (1.0f - slopeTime))
            {
                float mappedPhase = 0.5f + ((phase - (1.0f - slopeTime)) / slopeTime) * 0.5f;
                return evaluateWindow(mode, mappedPhase);
            }

            return 1.0f;
        }

        inline float evaluateTraversalCurve(TraversalMode mode, float phase, float frequencyHz) noexcept
        {
            if(mode == TraversalMode::None)
                return 0.5f;

            //const float visualPeriods = map(frequencyHz, 0.01f, 20.0f, 1.0f, 8.0f);
            const float p = std::fmod(phase * frequencyHz, 1.0f);
            float bipolarVal = 0.0f;

            switch(mode)
            {
                case TraversalMode::Sine:
                    bipolarVal = std::sin(twoPi * p);
                    break;

                case TraversalMode::Triangle:
                    bipolarVal = 2.0f * std::abs(2.0f * std::fmod(p + 0.75f, 1.0f) - 1.0f) - 1.0f;
                    break;

                case TraversalMode::Square:
                    bipolarVal = (p < 0.5f) ? 1.0f : -1.0f;
                    break;

                case TraversalMode::Random: // fake random
                    const float step = std::floor(phase * frequencyHz * 4.0f);
                    bipolarVal = std::fmod(std::sin(step * 12.9898f) * 43758.5453f, 2.0f) * 0.5f;
                    break;
            }

            return 0.5f + (bipolarVal * 0.5f);
        }
    }

}