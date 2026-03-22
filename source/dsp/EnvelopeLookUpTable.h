#pragma once

#include "../framework/Constants.h"
#include "../framework/Core.h"
#include "../utils/enum/EnvelopeMode.h"

// LUT TABLE
// fadeIn : 0 -> 0.5
// fadeOut : 0.5 -> 1

// This class just builds and holds tables datas.
// It is not responsible of computing the phase.
namespace particules
{
    class EnvelopeLookUpTable
    {
    public:
        EnvelopeLookUpTable();
        ~EnvelopeLookUpTable() = default;

        const float getEnvelopeValue(float phase);
        void setEnvelopeMode(EnvelopeMode m) noexcept { envMode = m; }

    private:
        static constexpr int SIZE = maxLutSize;

        void initTableData();
        void initTablePtr();

        const float smoothedStep(float x) const noexcept { return x * x * (3.f - 2.f * x); };
        void initHann() noexcept;
        void initGaussian() noexcept;
        void initTriangle() noexcept;
        void initExp() noexcept;
        void initSmoothed() noexcept;
        void initBlackman() noexcept;
        void initBlackmanHarris() noexcept;

        EnvelopeMode envMode;
        juce::dsp::LookupTable<float> lut;
        std::array<const float*, 7> tables;

        std::array<float, SIZE> hannTable;
        std::array<float, SIZE> gaussianTable;
        std::array<float, SIZE> blackmanTable;
        std::array<float, SIZE> blackmanHarrisTable;
        std::array<float, SIZE> triangleTable;
        std::array<float, SIZE> expTable;
        std::array<float, SIZE> smoothedTable;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeLookUpTable)
    };
}