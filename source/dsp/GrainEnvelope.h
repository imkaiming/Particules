#pragma once

#include "../framework/PluginParams.h"
#include "../framework/Core.h"
#include "../framework/audio/LookUpTable.h"
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
        static constexpr int SIZE = params::maxLutSize;

        void initTableData();
        void initTablePtr();

        /*
        static void initHann(std::span<float>) noexcept;
        static void initGaussian(std::span<float>) noexcept;
        static void initLinear(std::span<float>) noexcept;
        static void initExp(std::span<float>) noexcept;
        static void initSqrt(std::span<float>) noexcept;
        */


        //static void initSmoothed(std::span<float>) noexcept;
        //static const float smoothedStep(float x) noexcept { return x * x * (3.f - 2.f * x); }
        //static void initBlackman(std::span<float>) noexcept;
        //static void initBlackmanHarris(std::span<float>) noexcept;

        EnvelopeMode envMode;
        std::array<LookUpTable*, static_cast<int>(EnvelopeMode::Count)> tables;

        LookUpTable hannLUT;
        LookUpTable linearLUT;
        LookUpTable sqrtLUT;
        LookUpTable gaussianLUT;
        LookUpTable expLUT;
        //LookUpTable blackmanLUT;
        //LookUpTable blackmanHarrisLUT;
        //LookUpTable smoothedLUT;
    };
}