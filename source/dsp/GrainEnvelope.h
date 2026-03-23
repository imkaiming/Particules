#pragma once

#include "../framework/Constants.h"
#include "../framework/Core.h"
#include "../framework/LookUpTable.h"
#include "../framework/PhaseGenerator.h"
#include "../utils/enum/EnvelopeMode.h"

// LUT TABLE
// fadeIn : 0 -> 0.5
// sustain holds 0.5
// fadeOut : 0.5 -> 1

// This class just builds and holds tables datas.
// It is not responsible of computing the phase.
namespace particules
{
    class GrainEnvelope
    {
    public:
        GrainEnvelope();
        ~GrainEnvelope() = default;

        const float getEnvelopeValue(float phase) const noexcept;
        void setEnvelopeMode(EnvelopeMode m) noexcept { envMode = m; }

    private:
        static constexpr int SIZE = maxLutSize;

        void initTableData();
        void initTablePtr();

        static const float smoothedStep(float x) noexcept { return x * x * (3.f - 2.f * x); }
        static void initHann(std::span<float>) noexcept;
        static void initGaussian(std::span<float>) noexcept;
        static void initTriangle(std::span<float>) noexcept;
        static void initExp(std::span<float>) noexcept;
        static void initSmoothed(std::span<float>) noexcept;
        static void initBlackman(std::span<float>) noexcept;
        static void initBlackmanHarris(std::span<float>) noexcept;

        EnvelopeMode envMode;
        std::array<LookUpTable*, 7> tables;

        LookUpTable hannLUT;
        LookUpTable gaussianLUT;
        LookUpTable blackmanLUT;
        LookUpTable blackmanHarrisLUT;
        LookUpTable triangleLUT;
        LookUpTable expLUT;
        LookUpTable smoothedLUT;

        //std::array<const float*, 7> tables;

        //std::array<float, SIZE> hannTable;
        //std::array<float, SIZE> gaussianTable;
        //std::array<float, SIZE> blackmanTable;
        //std::array<float, SIZE> blackmanHarrisTable;
        //std::array<float, SIZE> triangleTable;
        //std::array<float, SIZE> expTable;
        //std::array<float, SIZE> smoothedTable;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainEnvelope)
    };
}