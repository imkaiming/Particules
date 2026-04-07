#pragma once

#include "../framework/PluginParams.h"
#include "../framework/audio/LookUpTable.h"
#include "../framework/audio/PhaseGenerator.h"
#include "../utils/enum/TraversalMode.h"

// unipolar LFO modulator return normalized position [0.f, 1.f]

// This class builds and holds table data
// it is also responsible of computing values based on the sample rate
namespace particules
{
    class PositionModulator
    {
    public:
        PositionModulator();
        ~PositionModulator() = default;

        void setSampleRate(double sr);
        void setParameters(TraversalMode, float);

        void reset();
        void advanceBlock(int);
        const float getPhase();

    private:
        static constexpr int SIZE = params::maxLutSize;
        static constexpr int MODCOUNT = 4;

        // init tables
        void initTableData();
        void initTablePtr();


        std::array<LookUpTable*, MODCOUNT> positionTables;
        LookUpTable sineLUT;
        LookUpTable squareLUT;
        LookUpTable triangleLUT;
        LookUpTable randomLUT;

        PhaseGenerator phaseGen;

        static void initSine(std::span<float> table) noexcept;
        static void initSquare(std::span<float> table) noexcept;
        static void initTriangle(std::span<float> table) noexcept;
        static void initRandom(std::span<float> table) noexcept;

        float mSampleRate;
        TraversalMode mTraversalMod; // the mod type to compute
        float mTraversalFreq; // frequency of the traversal
        float mPhaseIncrement; // time step per samples
        float mPhaseAccumulator; // position of a bufferSize block samples

        //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PositionModulator)
    };
}