#pragma once

#include "../utils/GrainVisualizer.h"
#include "Core.h"
#include "EnvelopeMode.h"
#include "TraversalMode.h"
#include "ParameterSnapshot.h"
#include "SampleSource.h"

struct GranularView
{
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

class Grain; // TODO to delete
class ParameterView
{
public:
    ParameterView();
    ~ParameterView() = default;

    void init(ValueTreeState& apvts, double sampleRate);

    // APVTS param reads
    float getMix() const noexcept { return view.mix ? convertToPercentage(view.mix->load(std::memory_order_relaxed)) : 0.0f; }
    float getDecibelGain() const noexcept { return view.gain ? view.gain->load(std::memory_order_relaxed) : 0.0f; }
    float getLinearGain() const noexcept
    {
        return view.gain ? juce::Decibels::decibelsToGain(view.gain->load(std::memory_order_relaxed)) : 0.0f;
    }
    float getSpeed() const noexcept { return view.speed ? view.speed->load(std::memory_order_relaxed) : 0.0f; }
    float getNormalizedDuration() const noexcept { return view.duration ? view.duration->load(std::memory_order_relaxed) : 0.0f; }
    float getEmission() const noexcept { return view.emission ? view.emission->load(std::memory_order_relaxed) : 0.0f; }
    float getNormalizedWindowSelection() const noexcept
    {
        return view.selection ? view.selection->load(std::memory_order_relaxed) : 0.0f;
    }
    float getNormalizedStartPosition() const noexcept
    {
        return view.position ? view.position->load(std::memory_order_relaxed) : 0.0f;
    }
    float getNormalizedSustainRatio() const noexcept
    {
        return view.sustainRatio ? view.sustainRatio->load(std::memory_order_relaxed) : 0.0f;
    }
    float getTraversalFreq() const noexcept
    {
        return view.traversalFreq ? view.traversalFreq->load(std::memory_order_relaxed) : 0.0f;
    }

    EnvelopeMode getEnvelopeMode() const noexcept;
    TraversalMode getTraversalMode() const noexcept;

    // runFreq flags
    void setIsPlaying(bool b) noexcept { mIsPlaying.store(b, std::memory_order_relaxed); }
    void setIsGrainsEmpty(bool b) noexcept { mIsGrainsEmpty.store(b, std::memory_order_relaxed); }

    bool getIsPlaying() const noexcept { return mIsPlaying.load(std::memory_order_relaxed); }
    bool getIsGrainsEmpty() const noexcept { return mIsGrainsEmpty.load(std::memory_order_relaxed); }

    //const int getNumChannels() const noexcept;
    const int getNumSamples() const noexcept { return numSamples.load(std::memory_order_relaxed); }
    void setNumSamples(const int samples) noexcept { numSamples.store(samples, std::memory_order_relaxed); }
    const int getNumChannels() const noexcept { return numChannels.load(std::memory_order_relaxed); }
    void setNumChannels(const int samples) noexcept { numChannels.store(samples, std::memory_order_relaxed); }

    const double getSampleRate() const noexcept { return mSampleRate.load(std::memory_order_relaxed); }
    void setSampleRate(double sr) noexcept { mSampleRate.store(sr, std::memory_order_relaxed); }

    void setGrains(juce::Array<Grain*>* grains) noexcept { mGrains = grains; }
    juce::Array<Grain*>* getGrains() const noexcept { return mGrains; }

    void setGrainVisualizer(GrainVisualizer* gv) noexcept { mGrainVisualizer = gv; }
    GrainVisualizer* getGrainVisualizer() const noexcept { return mGrainVisualizer; }

    const GranularView& getView() const noexcept { return view; }
    const ParameterSnapshot getSnapshot() const noexcept;

    //void setSampleSource(std::shared_ptr<const SampleSource> source) noexcept
    //{
    //sampleSource.store(std::move(source), std::memory_order_release);
    //}
    //std::shared_ptr<const SampleSource> getSampleSource() const noexcept { return sampleSource.load(std::memory_order_acquire); }

    void setAudioSource(std::shared_ptr<const AudioBuffer> ib) noexcept;
    std::shared_ptr<const AudioBuffer> getAudioSource() const noexcept;

private:
    //std::atomic<std::shared_ptr<const SampleSource>> sampleSource;
    std::atomic<std::shared_ptr<const AudioBuffer>> inputBuffer;

    // runtime flags
    std::atomic<bool> mIsPlaying;
    std::atomic<bool> mIsGrainsEmpty;
    std::atomic<double> mSampleRate;
    std::atomic<int> numChannels;
    std::atomic<int> numSamples;

    GranularView view;

    // non-atomic pointers: only safe if not mutated concurrently
    std::atomic<juce::Array<Grain*>*> mGrains{nullptr};
    std::atomic<GrainVisualizer*> mGrainVisualizer{nullptr};
};