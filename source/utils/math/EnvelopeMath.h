#pragma once

#include "../../framework/Constants.h"
#include "MathConstants.h"

namespace particules
{
    static constexpr int SIZE = maxLutSize;

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