#include "EnvelopeLookUpTable.h"
#include "../utils/math/MathConstants.h"
#include "../utils/math/Lerp.h"

EnvelopeLookUpTable::EnvelopeLookUpTable() : envMode{EnvelopeMode::Hann}
{
    initTableData();
    initTablePtr();
}

void EnvelopeLookUpTable::initTableData()
{
    initHann();
    initGaussian();
    initBlackman();
    initBlackmanHarris();
    initTriangle();
    initExp();
    initSmoothed();
}
void EnvelopeLookUpTable::initTablePtr()
{
    tables[static_cast<int>(EnvelopeMode::Hann)] = hannTable.data();
    tables[static_cast<int>(EnvelopeMode::Gaussian)] = gaussianTable.data();
    tables[static_cast<int>(EnvelopeMode::Triangle)] = triangleTable.data();
    tables[static_cast<int>(EnvelopeMode::Exp)] = expTable.data();
    tables[static_cast<int>(EnvelopeMode::Blackman)] = blackmanTable.data();
    tables[static_cast<int>(EnvelopeMode::BlackmanHarris)] = blackmanHarrisTable.data();
    tables[static_cast<int>(EnvelopeMode::Smoothed)] = smoothedTable.data();
}

const float EnvelopeLookUpTable::getEnvelopeValue(float phase)
{
    const float* table = tables[(int)envMode];

    // interpolation
    const float x = std::clamp(phase, 0.f, 1.f) * (static_cast<float>(SIZE) - 1.f);
    const int i = std::min(static_cast<int>(x), SIZE - 2);

    const float frac = x - i;
    const float a = table[i];
    const float b = table[i + 1];

    //return a + frac * (b - a);
    return lerp(a, b, frac);
}

void EnvelopeLookUpTable::initHann() noexcept
{
    const float phase = twoPi / static_cast<float>(SIZE - 1.f);
    for(size_t i = 0; i < SIZE; i++)
        hannTable[i] = 0.5f - 0.5f * std::cos(phase * static_cast<float>(i));

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

void EnvelopeLookUpTable::initGaussian() noexcept
{
    const float inv = 1.f / (SIZE - 1);
    const float sigma = 0.18f;
    const float invSigma = 1.f / sigma;

    for(size_t i = 0; i < SIZE; ++i)
    {
        const float x = (2.f * (i * inv) - 1.f) * invSigma;
        gaussianTable[i] = std::clamp(std::exp(-0.5f * x * x), 0.f, 1.f);
    }

    gaussianTable[0] = 0.f;
    gaussianTable[SIZE - 1] = 0.f;

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

void EnvelopeLookUpTable::initTriangle() noexcept
{
    const float center = 0.5f * static_cast<float>(SIZE - 1.f);
    const float invCenter = 1.f / center;

    for(size_t i = 0; i < SIZE; ++i)
    {
        const float x = std::abs(i - center) * invCenter;
        triangleTable[i] = 1.f - x;
    }

    triangleTable[0] = 0.f;
    triangleTable[SIZE - 1] = 0.f;

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
void EnvelopeLookUpTable::initExp() noexcept
{
    const float inv = 1.f / (SIZE - 1);
    const float k = 6.f;

    for(size_t i = 0; i < SIZE; ++i)
    {
        const float x = std::abs(2.f * (i * inv) - 1.f);
        expTable[i] = std::exp(-k * x);
    }

    expTable[0] = 0.f;
    expTable[SIZE - 1] = 0.f;

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

void EnvelopeLookUpTable::initSmoothed() noexcept
{
    const float inv = 1.f / static_cast<float>(SIZE - 1);

    for(size_t i = 0; i < SIZE; ++i)
    {
        const float x = static_cast<float>(i) * inv;
        if(x <= 0.5f)
            smoothedTable[i] = smoothedStep(2.f * x);
        else
            smoothedTable[i] = smoothedStep(2.f - 2.f * x);
    }

    //static bool firstTime = true;
    //if(firstTime)
    //{
    //    firstTime = false;
    //    DBG("SMOOTHED");
    //    DBG("Envelope first 5: " << smoothedTable[0] << ", " << smoothedTable[1] << ", " << smoothedTable[2] << ", "
    //                             << smoothedTable[3] << ", " << smoothedTable[4]);
    //    DBG("Envelope last 5: " << smoothedTable[SIZE - 5] << ", " << smoothedTable[SIZE - 4] << ", " << smoothedTable[SIZE - 3]
    //                            << ", " << smoothedTable[SIZE - 2] << ", " << smoothedTable[SIZE - 1]);
    //}
}

void EnvelopeLookUpTable::initBlackman() noexcept
{
    constexpr float a0 = 0.42f;
    constexpr float a1 = 0.50f;
    constexpr float a2 = 0.08f;

    const float inv = 1.f / static_cast<float>(SIZE - 1);

    for(size_t i = 0; i < SIZE; ++i)
    {
        const float phase = twoPi * static_cast<float>(i) * inv;
        const float val = a0 - a1 * std::cos(phase) + a2 * std::cos(2.f * phase);
        blackmanTable[i] = std::clamp(val, 0.f, 1.f);
    }

    blackmanTable[0] = 0.f;
    blackmanTable[SIZE - 1] = 0.f;

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

void EnvelopeLookUpTable::initBlackmanHarris() noexcept
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
        blackmanHarrisTable[i] = std::clamp(val, 0.f, 1.f);
    }

    blackmanHarrisTable[0] = 0.f;
    blackmanHarrisTable[SIZE - 1] = 0.f;

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
