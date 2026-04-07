#pragma once

#include "../../framework/PluginParams.h"
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
            switch(mode)
            {
                case EnvelopeMode::Hann:
                    return 0.5f - 0.5f * std::cos(twoPi * phase);
                case EnvelopeMode::Sqrt:
                    return std::sqrt(1.f - std::abs(2.f * phase - 1.f));
                // ... autres modes
                default:
                    return 0.f;
            }
        }

        // Fonction qui simule le comportement exact de ton GrainProcessor
        inline float evaluateEnvelopeWithPlateau(EnvelopeMode mode, float phase, float sustainRatio) noexcept
        {
            // Si pas de plateau, on évalue la fenêtre normalement
            if(sustainRatio <= 0.0f)
                return evaluateWindow(mode, phase);

            // Calcul des proportions (ex: sustainRatio = 0.5 signifie 50% de plateau)
            // L'attaque et le release se partagent le reste du temps
            const float slopeTime = (1.0f - sustainRatio) * 0.5f;

            if(phase < slopeTime)
            {
                // Phase d'attaque : on remappe la phase de [0, slopeTime] vers [0, 0.5]
                float mappedPhase = (phase / slopeTime) * 0.5f;
                return evaluateWindow(mode, mappedPhase);
            }
            else if(phase > (1.0f - slopeTime))
            {
                // Phase de release : on remappe la phase de [1-slopeTime, 1] vers [0.5, 1.0]
                float mappedPhase = 0.5f + ((phase - (1.0f - slopeTime)) / slopeTime) * 0.5f;
                return evaluateWindow(mode, mappedPhase);
            }
            else
            {
                // Plateau
                return 1.0f;
            }
        }
    }
}