#pragma once

#include "../utils/GrainVisualizer.h"
#include "Core.h"
#include "ParameterSnapshot.h"
#include "SampleSource.h"

struct GranularView
{
    std::atomic<float>* density = nullptr;
    std::atomic<float>* duration = nullptr;
    std::atomic<float>* speed = nullptr;
    std::atomic<float>* mix = nullptr;
    std::atomic<float>* gain = nullptr;

    std::atomic<float>* position = nullptr;
    std::atomic<float>* selection = nullptr;
    std::atomic<float>* sustainRatio = nullptr;

    std::atomic<float>* envType = nullptr;
    std::atomic<float>* traversalMode = nullptr;
    std::atomic<float>* traversalTime = nullptr;
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
    float getGain() const noexcept { return view.gain ? view.gain->load(std::memory_order_relaxed) : 0.0f; }
    float getDecibelToGain() const noexcept
    {
        return view.gain ? juce::Decibels::decibelsToGain(view.gain->load(std::memory_order_relaxed)) : 0.0f;
    }
    float getSpeed() const noexcept { return view.speed ? view.speed->load(std::memory_order_relaxed) : 0.0f; }
    float getDuration() const noexcept { return view.duration ? view.duration->load(std::memory_order_relaxed) : 0.0f; }
    float getDensity() const noexcept { return view.density ? view.density->load(std::memory_order_relaxed) : 0.0f; }
    float getWindowSelection() const noexcept { return view.selection ? view.selection->load(std::memory_order_relaxed) : 0.0f; }
    float getFilePosition() const noexcept { return view.position ? view.position->load(std::memory_order_relaxed) : 0.0f; }
    float getSustainRatio() const noexcept
    {
        return view.sustainRatio ? view.sustainRatio->load(std::memory_order_relaxed) : 0.0f;
    }
    float getTraversalTime() const noexcept
    {
        return view.traversalTime ? view.traversalTime->load(std::memory_order_relaxed) : 0.0f;
    }

    float getEnvelopeType() const noexcept { return view.envType ? (view.envType->load(std::memory_order_relaxed)) : 0; }
    float getTraversalMode() const noexcept
    {
        return view.traversalMode ? (view.traversalMode->load(std::memory_order_relaxed)) : 0;
    }

    // runtime flags
    void setIsPlaying(bool b) noexcept { mIsPlaying.store(b, std::memory_order_relaxed); }
    void setIsGrainsEmpty(bool b) noexcept { mIsGrainsEmpty.store(b, std::memory_order_relaxed); }

    bool getIsPlaying() const noexcept { return mIsPlaying.load(std::memory_order_relaxed); }
    bool getIsGrainsEmpty() const noexcept { return mIsGrainsEmpty.load(std::memory_order_relaxed); }

    const int getNumChannels() const noexcept;
    const int getNumSamples() const noexcept;

    const double getSampleRate() const noexcept { return mSampleRate.load(std::memory_order_relaxed); }
    void setSampleRate(double sr) noexcept { mSampleRate.store(sr, std::memory_order_relaxed); }

    void setGrains(juce::Array<Grain*>* grains) noexcept { mGrains = grains; }
    juce::Array<Grain*>* getGrains() const noexcept { return mGrains; }

    void setGrainVisualizer(GrainVisualizer* gv) noexcept { mGrainVisualizer = gv; }
    GrainVisualizer* getGrainVisualizer() const noexcept { return mGrainVisualizer; }

    const GranularView& getView() const noexcept { return view; }
    const ParameterSnapshot getSnapshot() const noexcept;

    //void setSampleSource(const SampleSource* newSource) noexcept { sampleSource.store(newSource, std::memory_order_relaxed); };
    //const SampleSource* getSampleSource() const noexcept { return sampleSource.load(std::memory_order_relaxed); };
    void setSampleSource(std::shared_ptr<const SampleSource> source) noexcept
    {
        sampleSource.store(std::move(source), std::memory_order_release);
    }
    std::shared_ptr<const SampleSource> getSampleSource() const noexcept { return sampleSource.load(std::memory_order_acquire); }

private:
    std::atomic<std::shared_ptr<const SampleSource>> sampleSource;

    // runtime flags
    std::atomic<bool> mIsPlaying;
    std::atomic<bool> mIsGrainsEmpty;
    std::atomic<double> mSampleRate;

    GranularView view;

    // non-atomic pointers: only safe if not mutated concurrently
    std::atomic<juce::Array<Grain*>*> mGrains{nullptr};
    std::atomic<GrainVisualizer*> mGrainVisualizer{nullptr};
};