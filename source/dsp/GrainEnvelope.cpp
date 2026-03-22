#include "GrainEnvelope.h"

#include "../utils/math/Lerp.h"
#include "../utils/math/MathConstants.h"

namespace particules
{
    GrainEnvelope::GrainEnvelope() : envMode{EnvelopeMode::Hann}
    {
        initTableData();
        initTablePtr();
    }

    void GrainEnvelope::initTableData()
    {
        hannLUT.populate(GrainEnvelope::initHann);
        gaussianLUT.populate(GrainEnvelope::initGaussian);
        blackmanLUT.populate(GrainEnvelope::initBlackman);
        blackmanHarrisLUT.populate(GrainEnvelope::initBlackmanHarris);
        triangleLUT.populate(GrainEnvelope::initTriangle);
        expLUT.populate(GrainEnvelope::initExp);
        smoothedLUT.populate(GrainEnvelope::initSmoothed);
    }
    void GrainEnvelope::initTablePtr()
    {
        tables[static_cast<int>(EnvelopeMode::Hann)] = &hannLUT;
        tables[static_cast<int>(EnvelopeMode::Gaussian)] = &gaussianLUT;
        tables[static_cast<int>(EnvelopeMode::Triangle)] = &triangleLUT;
        tables[static_cast<int>(EnvelopeMode::Exp)] = &expLUT;
        tables[static_cast<int>(EnvelopeMode::Blackman)] = &blackmanLUT;
        tables[static_cast<int>(EnvelopeMode::BlackmanHarris)] = &blackmanHarrisLUT;
        tables[static_cast<int>(EnvelopeMode::Smoothed)] = &smoothedLUT;

        //tables[static_cast<int>(EnvelopeMode::Hann)] = hannTable.data();
        //tables[static_cast<int>(EnvelopeMode::Gaussian)] = gaussianTable.data();
        //tables[static_cast<int>(EnvelopeMode::Triangle)] = triangleTable.data();
        //tables[static_cast<int>(EnvelopeMode::Exp)] = expTable.data();
        //tables[static_cast<int>(EnvelopeMode::Blackman)] = blackmanTable.data();
        //tables[static_cast<int>(EnvelopeMode::BlackmanHarris)] = blackmanHarrisTable.data();
        //tables[static_cast<int>(EnvelopeMode::Smoothed)] = smoothedTable.data();
    }

    const float GrainEnvelope::getEnvelopeValue(float phase)
    {
        const LookUpTable* table = tables[(int)envMode];
        return table->getValue(phase);
        /*
        // interpolation
        const float x = std::clamp(phase, 0.f, 1.f) * (static_cast<float>(SIZE) - 1.f);
        const int i = std::min(static_cast<int>(x), SIZE - 2);

        const float frac = x - i;
        const float a = table[i];
        const float b = table[i + 1];

        //return a + frac * (b - a);
        return lerp(a, b, frac);
        */
    }

    void GrainEnvelope::initHann(std::span<float> table) noexcept
    {
        const float phase = twoPi / static_cast<float>(SIZE - 1.f);
        for(size_t i = 0; i < SIZE; i++)
            table[i] = 0.5f - 0.5f * std::cos(phase * static_cast<float>(i));

        //static bool firstTime = true;
        //if(firstTime)
        //{
        //    firstTime = false;
        //    DBG("HANN");
        //    DBG("Envelope first 5: " << hannTable[0] << ", " << hannTable[1] << ", " << hannTable[2] << ", " << hannTable[3] << ", "
        //                             << hannTable[4]);
        //    DBG("Envelope last 5: " << hannTable[SIZE - 5] << ", " << hannTable[SIZE - 4] << ", " << hannTable[SIZE - 3] << ", "
        //                            << hannTable[SIZE - 2] << ", " << hannTable[SIZE - 1]);
        //}
    }

    void GrainEnvelope::initGaussian(std::span<float> table) noexcept
    {
        const float inv = 1.f / (SIZE - 1);
        const float sigma = 0.18f;
        const float invSigma = 1.f / sigma;

        for(size_t i = 0; i < SIZE; ++i)
        {
            const float x = (2.f * (i * inv) - 1.f) * invSigma;
            table[i] = std::clamp(std::exp(-0.5f * x * x), 0.f, 1.f);
        }

        table[0] = 0.f;
        table[SIZE - 1] = 0.f;

        //static bool firstTime = true;
        //if(firstTime)
        //{
        //    firstTime = false;
        //    DBG("GAUSSIAN");
        //    DBG("Envelope first 5: " << gaussianTable[0] << ", " << gaussianTable[1] << ", " << gaussianTable[2] << ", "
        //                             << gaussianTable[3] << ", " << gaussianTable[4]);
        //    DBG("Envelope last 5: " << gaussianTable[SIZE - 5] << ", " << gaussianTable[SIZE - 4] << ", " << gaussianTable[SIZE - 3]
        //                            << ", " << gaussianTable[SIZE - 2] << ", " << gaussianTable[SIZE - 1]);
        //}
    }

    void GrainEnvelope::initTriangle(std::span<float> table) noexcept
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

        //static bool firstTime = true;
        //if(firstTime)
        //{
        //    firstTime = false;
        //    DBG("TRIANGLE");
        //    DBG("Envelope first 5: " << triangleTable[0] << ", " << triangleTable[1] << ", " << triangleTable[2] << ", "
        //                             << triangleTable[3] << ", " << triangleTable[4]);
        //    DBG("Envelope last 5: " << triangleTable[SIZE - 5] << ", " << triangleTable[SIZE - 4] << ", " << triangleTable[SIZE - 3]
        //                            << ", " << triangleTable[SIZE - 2] << ", " << triangleTable[SIZE - 1]);
        //}
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
    }

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

        //static bool firstTime = true;
        //if(firstTime)
        //{
        //    firstTime = false;
        //    DBG("SMOOTHED");
        //    DBG("Envelope first 5: " << table[0] << ", " << smoothedTable[1] << ", " << smoothedTable[2] << ", "
        //                             << smoothedTable[3] << ", " << smoothedTable[4]);
        //    DBG("Envelope last 5: " << smoothedTable[SIZE - 5] << ", " << smoothedTable[SIZE - 4] << ", " << smoothedTable[SIZE - 3]
        //                            << ", " << smoothedTable[SIZE - 2] << ", " << smoothedTable[SIZE - 1]);
        //}
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

        //static bool firstTime = true;
        //if(firstTime)
        //{
        //    firstTime = false;
        //    DBG("BLACKMAN");
        //    DBG("Envelope first 5: " << blackmanTable[0] << ", " << blackmanTable[1] << ", " << blackmanTable[2] << ", "
        //                             << blackmanTable[3] << ", " << blackmanTable[4]);
        //    DBG("Envelope last 5: " << blackmanTable[SIZE - 5] << ", " << blackmanTable[SIZE - 4] << ", " << blackmanTable[SIZE - 3]
        //                            << ", " << blackmanTable[SIZE - 2] << ", " << blackmanTable[SIZE - 1]);
        //}
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

        //    static bool firstTime = true;
        //    if(firstTime)
        //    {
        //        firstTime = false;
        //        DBG("BLACKMAN HARRIS");
        //        DBG("Envelope first 5: " << blackmanHarrisTable[0] << ", " << blackmanHarrisTable[1] << ", " << blackmanHarrisTable[2]
        //                                 << ", " << blackmanHarrisTable[3] << ", " << blackmanHarrisTable[4]);
        //        DBG("Envelope last 5: " << blackmanHarrisTable[SIZE - 5] << ", " << blackmanHarrisTable[SIZE - 4] << ", "
        //                                << blackmanHarrisTable[SIZE - 3] << ", " << blackmanHarrisTable[SIZE - 2] << ", "
        //                                << blackmanHarrisTable[SIZE - 1]);
        //    }
    }
}