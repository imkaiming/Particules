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
        hannLUT.populate(dsp::initHann);
        linearLUT.populate(dsp::initLinear);
        sqrtLUT.populate(dsp::initSqrt);
        gaussianLUT.populate(dsp::initGaussian);
        expLUT.populate(dsp::initExp);
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
    const float GrainEnvelope::getEnvelopeModeValue(int mode, float phase) noexcept { return (tables[mode]->getValue(phase)); }

}
