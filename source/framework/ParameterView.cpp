#include "ParameterView.h"
#include "../utils/PluginParams.h"

namespace particules
{
    ParameterView::ParameterView() : mIsGrainsEmpty{true}, mIsPlaying{false}, mSampleRate{0.0} {}

    void ParameterView::init(ValueTreeState& apvts, double sampleRate)
    {
        setSampleRate(sampleRate);
        //mSampleRate.store(sampleRate, std::memory_order_relaxed);
        mix = apvts.getRawParameterValue(global::mix::id);
        output = apvts.getRawParameterValue(global::output::id);
        emission = apvts.getRawParameterValue(grains::emission::id);
        duration = apvts.getRawParameterValue(grains::duration::id);
        speed = apvts.getRawParameterValue(grains::speed::id);
        position = apvts.getRawParameterValue(global::position::id);
        selection = apvts.getRawParameterValue(global::selection::id);
        envMode = apvts.getRawParameterValue(grains::envelopeMode::id);
        sustainRatio = apvts.getRawParameterValue(grains::sustainRatio::id);
        traversalMode = apvts.getRawParameterValue(grains::traversalMode::id);
        traversalFreq = apvts.getRawParameterValue(grains::traversalFreq::id);
    }

    EnvelopeMode ParameterView::getEnvelopeMode() const noexcept
    {
        if(!envMode)
            return EnvelopeMode::Hann;

        const float v = envMode ? (envMode->load(std::memory_order_relaxed)) : 0.f;
        const int choice = static_cast<int>(std::round(v));

        if(choice < 0 || choice >= 7)
            return EnvelopeMode::Hann;

        return static_cast<EnvelopeMode>(choice);
    }

    TraversalMode ParameterView::getTraversalMode() const noexcept
    {
        if(!traversalMode)
            return TraversalMode::Sine;

        const float v = traversalMode ? (traversalMode->load(std::memory_order_relaxed)) : 0;
        const int choice = static_cast<int>(std::round(v));
        if(choice <= 0 || choice > 5)
            return TraversalMode::Sine;

        return static_cast<TraversalMode>(choice);
    }

    const ParameterSnapshot ParameterView::getSnapshot() const noexcept
    {
        ParameterSnapshot snapshot;

        // get Buffer data
        snapshot.sampleRate = getSampleRate();
        snapshot.inputNumSamples = getNumSamples();
        snapshot.inputNumChannels = getNumChannels();

        // position data
        snapshot.startPositionSamples = static_cast<int>(getNormalizedStartPosition() * snapshot.inputNumSamples);
        snapshot.selectionSamples = static_cast<int>(getNormalizedWindowSelection() * snapshot.inputNumSamples);

        // time data
        snapshot.durationSamples = static_cast<int>(getNormalizedDuration() * snapshot.sampleRate);

        // grain data
        snapshot.mix = getMix();
        snapshot.speed = getSpeed();
        snapshot.emission = getEmission();
        snapshot.envMode = getEnvelopeMode();
        snapshot.sustainRatio = getNormalizedSustainRatio();
        snapshot.linearGain = getLinearGain();
        snapshot.traversalMode = getTraversalMode();
        snapshot.traversalFreq = getTraversalFreq();

        return snapshot;
    };
}
