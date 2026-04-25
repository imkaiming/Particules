#include "PositionModulator.h"

#include <juce_core/juce_core.h>

#include "utils/math/MathConstants.h"

// quand on change le mod on reset la position de la phase.
// quand on met pause on reset egalement le modulator
// on calcule chaque avancer en fonction du bufferSize
namespace particules
{
    PositionModulator::PositionModulator()
        : mPhaseAccumulator{0.f}, mTraversalMod{TraversalMode::Sine}, mTraversalFreq{1.f}, mSampleRate{0.0}
    {
        initTableData();
        initTablePtr();
    }

    void PositionModulator::initTableData()
    {
        sineLUT.populate(PositionModulator::initSine);
        triangleLUT.populate(PositionModulator::initTriangle);
        squareLUT.populate(PositionModulator::initSquare);
        randomLUT.populate(PositionModulator::initRandom);

        //const float invSize = 1.f / (float)SIZE;
        //for(int i = 0; i < SIZE; ++i)
        //{
        //    const float val = static_cast<float>(i) * invSize;
        //    sineLUT[i] = getUnipolarSine(val);
        //    squareLUT[i] = getUnipolarSquare(val);
        //    triangleLUT[i] = getUnipolarTriangular(val);
        //    randomLUT[i] = r.nextFloat();
        //}
    }

    void PositionModulator::initTablePtr()
    {
        positionTables[static_cast<int>(TraversalMode::Sine)] = &sineLUT;
        positionTables[static_cast<int>(TraversalMode::Triangle)] = &triangleLUT;
        positionTables[static_cast<int>(TraversalMode::Square)] = &squareLUT;
        positionTables[static_cast<int>(TraversalMode::Random)] = &randomLUT;
    }

    void PositionModulator::reset() { phaseGen.reset(); }

    void PositionModulator::setSampleRate(double sr) { phaseGen.setSampleRate(sr); }

    void PositionModulator::advanceBlock(int numSamples) { phaseGen.advance(numSamples); }

    void PositionModulator::setParameters(TraversalMode traversalMod, float traversalFreq)
    {
        if(mTraversalMod != traversalMod)
        {
            reset();
            mTraversalMod = traversalMod;
        }
        phaseGen.setFrequency(traversalFreq);
    }

    const float PositionModulator::getPhase()
    {
        if(mTraversalMod == TraversalMode::None)
            return 0.f;

        const LookUpTable* table = positionTables[static_cast<int>(mTraversalMod)];

        return table->getValue(phaseGen.getPhase());
    }

    // init functions
    void PositionModulator::initSine(std::span<float> table) noexcept
    {
        const float invSize = 1.f / (float)table.size();
        for(int i = 0; i < table.size(); ++i)
        {
            const float phase = static_cast<float>(i) * invSize;
            table[i] = 0.5f * (std::sin(twoPi * phase) + 1.f);
        }
    }
    void PositionModulator::initSquare(std::span<float> table) noexcept
    {
        const float invSize = 1.f / (float)table.size();
        for(int i = 0; i < table.size(); ++i)
        {
            const float phase = static_cast<float>(i) * invSize;
            const float p = phase - std::floor(phase);
            table[i] = (p >= 0.5f) ? 1.f : 0.f;
        }
    }
    void PositionModulator::initTriangle(std::span<float> table) noexcept
    {
        const float invSize = 1.f / (float)table.size();
        for(int i = 0; i < table.size(); ++i)
        {
            const float phase = static_cast<float>(i) * invSize;
            const float p = phase - std::floor(phase);
            table[i] = 1.0f - std::fabs(p * 2.0f - 1.0f);
        }
    }

    void PositionModulator::initRandom(std::span<float> table) noexcept
    {
        static juce::Random r;
        for(int i = 0; i < table.size(); ++i)
        {
            table[i] = r.nextFloat();
        }
    }
}
// mod types

//    float PositionModulator::getUnipolarCos(float normalizedPhase) { return 0.5f * (std::cos(twoPi * normalizedPhase) + 1.f); }
//
//    float PositionModulator::getUnipolarSine(float normalizedPhase) { return 0.5f * (std::sin(twoPi * normalizedPhase) + 1.0f); }
//
//    float PositionModulator::getUnipolarTriangular(float normalizedPhase)
//    {
//        const float p = normalizedPhase - std::floor(normalizedPhase);
//        return 1.0f - std::fabs(p * 2.0f - 1.0f);
//    }
//
//    float PositionModulator::getUnipolarSquare(float normalizedPhase)
//    {
//        const float p = normalizedPhase - std::floor(normalizedPhase);
//        return (p >= 0.5f);
//    }
//}
//float PositionModulator::getRandom()
//{
//    static std::mt19937 rng(std::random_device{}());
//    static std::uniform_real_distribution<float> dist(0.f, 1.f);
//    return dist(rng);
//}

/*
float PositionModulator::getPhaseAtOffset(int offset)
{
    float phase = mPhaseAccumulator + offset * mPhaseIncrement;
    phase -= std::floor(phase);
    return phase;
}
*/
/*
float PositionModulator::computePhaseAtOffset(int offset)
{
    float normalizedPhase = getPhaseAtOffset(offset);
    float value = 0.f;
    switch(mTraversalMod)
    {
        case 1:
            value = getUnipolarSine(normalizedPhase);
            break;
        case 2:
            value = getUnipolarSquare(normalizedPhase);
            break;
        case 3:
            value = getUnipolarTriangular(normalizedPhase);
            break;
        case 4:
            value = getRandom();
            break;
        case 5:
            value = 0.f;
            break;
        default:
            std::logic_error("PositionModulator mTraversalMod should be 1 to 5");
            break;
    }
    return value;
}
*/
