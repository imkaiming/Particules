#include "GrainEnvelope.h"

#include "../utils/math/EnvelopeMath.h"


namespace particules
{
    GrainEnvelope::GrainEnvelope() : envMode{EnvelopeMode::Hann}
    {
        initTableData();
        initTablePtr();
    }

    void GrainEnvelope::initTableData()
    {
        hannLUT.populate(particules::initHann);
        linearLUT.populate(particules::initLinear);
        sqrtLUT.populate(particules::initSqrt);
        gaussianLUT.populate(particules::initGaussian);
        expLUT.populate(particules::initExp);
    }

    void GrainEnvelope::initTablePtr()
    {
        tables[static_cast<int>(EnvelopeMode::Hann)] = &hannLUT;
        tables[static_cast<int>(EnvelopeMode::Linear)] = &linearLUT;
        tables[static_cast<int>(EnvelopeMode::Sqrt)] = &sqrtLUT;
        tables[static_cast<int>(EnvelopeMode::Gaussian)] = &gaussianLUT;
        tables[static_cast<int>(EnvelopeMode::Exp)] = &expLUT;
        //tables[static_cast<int>(EnvelopeMode::Blackman)] = &blackmanLUT;
        //tables[static_cast<int>(EnvelopeMode::BlackmanHarris)] = &blackmanHarrisLUT;
        //tables[static_cast<int>(EnvelopeMode::Smoothed)] = &smoothedLUT;

    }

    const float GrainEnvelope::getEnvelopeValue(float phase) const noexcept
    { 
        const LookUpTable* table = tables[(int)envMode];
        assert(table != nullptr);
        return table->getValue(phase);
    }
    /*
    void GrainEnvelope::initHann(std::span<float> table) noexcept
    {
        const float p = twoPi / static_cast<float>(SIZE - 1);
        for(size_t i = 0; i < SIZE; i++)
            table[i] = 0.5f - 0.5f * std::cos(p * static_cast<float>(i));

    }

    void GrainEnvelope::initGaussian(std::span<float> table) noexcept
    {
        const float inv = 1.f / (SIZE - 1);
        const float sigma = 0.18f;
        const float invSigma = 1.f / sigma;

        for(size_t i = 0; i < SIZE; ++i)
        {
            const float x = (2.f * (i * inv) - 1.f) * invSigma;
            table[i] = std::exp(-0.5f * x * x);
            //table[i] = std::clamp(std::exp(-0.5f * x * x), 0.f, 1.f);
        }

        table[0] = 0.f;
        table[SIZE - 1] = 0.f;

    }

    void GrainEnvelope::initLinear(std::span<float> table) noexcept
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
    void GrainEnvelope::initExp(std::span<float> table) noexcept
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

    void GrainEnvelope::initSqrt(std::span<float> table) noexcept
    {
        const float inv = 1.f / (SIZE - 1);
        for(size_t i = 0; i < SIZE; ++i)
        {
            float x = static_cast<float>(i) * inv;
            table[i] = std::sqrt(x);

            // Option B : quarter sine
            // table[i] = std::sin(x * 1.570796f);
        }
    }*/

    /*
    void GrainEnvelope::initSmoothed(std::span<float> table) noexcept
    {
        const float inv = 1.f / static_cast<float>(SIZE - 1);

        for(size_t i = 0; i < SIZE; ++i)
        {
            const float x = static_cast<float>(i) * inv;
            if(x <= 0.5f)
                table[i] = smoothedStep(2.f * x);
            else
                table[i] = smoothedStep(2.f - 2.f * x);
        }
    }

    void GrainEnvelope::initBlackman(std::span<float> table) noexcept
    {
        constexpr float a0 = 0.42f;
        constexpr float a1 = 0.50f;
        constexpr float a2 = 0.08f;

        const float inv = 1.f / static_cast<float>(SIZE - 1);

        for(size_t i = 0; i < SIZE; ++i)
        {
            const float phase = twoPi * static_cast<float>(i) * inv;
            const float val = a0 - a1 * std::cos(phase) + a2 * std::cos(2.f * phase);
            table[i] = std::clamp(val, 0.f, 1.f);
        }

        table[0] = 0.f;
        table[SIZE - 1] = 0.f;
    }

    void GrainEnvelope::initBlackmanHarris(std::span<float> table) noexcept
    {
        constexpr float a0 = 0.35875f;
        constexpr float a1 = 0.48829f;
        constexpr float a2 = 0.14128f;
        constexpr float a3 = 0.01168f;

        const float inv = 1.f / static_cast<float>(SIZE - 1);

        for(size_t i = 0; i < SIZE; ++i)
        {
            const float phase = twoPi * static_cast<float>(i) * inv;
            const float val = a0 - a1 * std::cos(phase) + a2 * std::cos(2.f * phase) - a3 * std::cos(3.f * phase);
            table[i] = std::clamp(val, 0.f, 1.f);
        }

        table[0] = 0.f;
        table[SIZE - 1] = 0.f;

    }
    */
}


        //static bool firstTime = true;
//if(firstTime)
//{
//    firstTime = false;
//    DBG("EXP");
//    DBG("Envelope first 5: " << expTable[0] << ", " << expTable[1] << ", " << expTable[2] << ", " << expTable[3] << ", "
//                             << expTable[4]);
//    DBG("Envelope last 5: " << expTable[SIZE - 5] << ", " << expTable[SIZE - 4] << ", " << expTable[SIZE - 3] << ", "
//                            << expTable[SIZE - 2] << ", " << expTable[SIZE - 1]);
//}