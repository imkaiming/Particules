#pragma once

#include "../framework/Core.h"
#include "../framework/EnvelopeMode.h"

// LUT TABLE
// fadeIn : 0 -> 0.5
// fadeOut : 0.5 -> 1
class EnvelopeLookUpTable
{
public:
    explicit EnvelopeLookUpTable();
    ~EnvelopeLookUpTable() = default;

    const float getEnvelopeValue(float phase);

    //float readHann(float phase) const;
    //float readGaussian(float phase) const;
    //float readTriangle(float phase) const;
    //float readExp(float phase) const;
    //float readSmoothed(float phase) const;
    //float readBlackman(float phase) const;
    //float readBlackmanHarris(float phase) const;

    void setEnvelopeMode(EnvelopeMode m) noexcept { envMode = m; }

private:
    static constexpr int SIZE = 2048;

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

    std::array<const float*, 7> tables;

    std::array<float, SIZE> hannTable;
    std::array<float, SIZE> gaussianTable;
    std::array<float, SIZE> blackmanTable;
    std::array<float, SIZE> blackmanHarrisTable;
    std::array<float, SIZE> triangleTable;
    std::array<float, SIZE> expTable;
    std::array<float, SIZE> smoothedTable;
};