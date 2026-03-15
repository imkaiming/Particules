#pragma once

#include "../utils/EnvelopeMode.h"
#include "../utils/ParameterSnapshot.h"
#include "../utils/TraversalMode.h"
#include "Core.h"

class ParameterView
{
public:
    ParameterView();
    ~ParameterView() = default;

    void init(ValueTreeState& apvts, double sampleRate);

    // APVTS param reads
    float getMix() const noexcept { return mix ? convertToPercentage(mix->load(std::memory_order_relaxed)) : 0.0f; }
    float getDecibelGain() const noexcept { return gain ? gain->load(std::memory_order_relaxed) : 0.0f; }
    float getLinearGain() const noexcept
    {
        return gain ? juce::Decibels::decibelsToGain(gain->load(std::memory_order_relaxed)) : 0.0f;
    }
    float getSpeed() const noexcept { return speed ? speed->load(std::memory_order_relaxed) : 0.0f; }
    float getNormalizedDuration() const noexcept { return duration ? duration->load(std::memory_order_relaxed) : 0.0f; }
    float getEmission() const noexcept { return emission ? emission->load(std::memory_order_relaxed) : 0.0f; }
    float getNormalizedWindowSelection() const noexcept { return selection ? selection->load(std::memory_order_relaxed) : 0.0f; }
    float getNormalizedStartPosition() const noexcept { return position ? position->load(std::memory_order_relaxed) : 0.0f; }
    float getNormalizedSustainRatio() const noexcept
    {
        return sustainRatio ? sustainRatio->load(std::memory_order_relaxed) : 0.0f;
    }
    float getTraversalFreq() const noexcept { return traversalFreq ? traversalFreq->load(std::memory_order_relaxed) : 0.0f; }

    EnvelopeMode getEnvelopeMode() const noexcept;
    TraversalMode getTraversalMode() const noexcept;

    // runtime flags
    void setIsPlaying(bool b) noexcept { mIsPlaying.store(b, std::memory_order_relaxed); }
    void setIsGrainsEmpty(bool b) noexcept { mIsGrainsEmpty.store(b, std::memory_order_relaxed); }

    bool getIsPlaying() const noexcept { return mIsPlaying.load(std::memory_order_relaxed); }
    bool getIsGrainsEmpty() const noexcept { return mIsGrainsEmpty.load(std::memory_order_relaxed); }

    const int getNumSamples() const noexcept { return numSamples.load(std::memory_order_relaxed); }
    void setNumSamples(const int samples) noexcept { numSamples.store(samples, std::memory_order_relaxed); }
    const int getNumChannels() const noexcept { return numChannels.load(std::memory_order_relaxed); }
    void setNumChannels(const int samples) noexcept { numChannels.store(samples, std::memory_order_relaxed); }

    const double getSampleRate() const noexcept { return mSampleRate.load(std::memory_order_relaxed); }
    void setSampleRate(double sr) noexcept { mSampleRate.store(sr, std::memory_order_relaxed); }

    const ParameterSnapshot getSnapshot() const noexcept;

    //void setAudioSource(std::shared_ptr<const AudioBuffer> ib) noexcept;
    //std::shared_ptr<const AudioBuffer> getAudioSource() const noexcept;

private:
    //std::atomic<std::shared_ptr<const AudioBuffer>> inputBuffer;
    //AtomicSharedPtr<const AudioBuffer> inputBuffer;

    // runtime flags
    std::atomic<bool> mIsPlaying;
    std::atomic<bool> mIsGrainsEmpty;
    std::atomic<double> mSampleRate;
    std::atomic<int> numChannels;
    std::atomic<int> numSamples;

    // apvts value
    std::atomic<float>* emission = nullptr;
    std::atomic<float>* duration = nullptr;
    std::atomic<float>* speed = nullptr;
    std::atomic<float>* mix = nullptr;
    std::atomic<float>* gain = nullptr;
    std::atomic<float>* position = nullptr;
    std::atomic<float>* selection = nullptr;
    std::atomic<float>* sustainRatio = nullptr;
    std::atomic<float>* envMode = nullptr;
    std::atomic<float>* traversalMode = nullptr;
    std::atomic<float>* traversalFreq = nullptr;
};