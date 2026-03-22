#include "ParameterView.h"
#include "../utils/PluginParams.h"

namespace particules
{
    ParameterView::ParameterView() : mIsGrainsEmpty{true}, mIsPlaying{false}, mSampleRate{0.0} {}

    void ParameterView::init(ValueTreeState& apvts, double sampleRate)
    {
        setSampleRate(sampleRate);
        //mSampleRate.store(sampleRate, std::memory_order_relaxed);
        mix = apvts.getRawParameterValue(Params::Mix::id);
        gain = apvts.getRawParameterValue(Params::Gain::id);
        emission = apvts.getRawParameterValue(Params::Emission::id);
        duration = apvts.getRawParameterValue(Params::Duration::id);
        speed = apvts.getRawParameterValue(Params::Speed::id);
        position = apvts.getRawParameterValue(Params::Position::id);
        selection = apvts.getRawParameterValue(Params::Selection::id);
        envMode = apvts.getRawParameterValue(Params::EnvelopeMode::id);
        sustainRatio = apvts.getRawParameterValue(Params::SustainRatio::id);
        traversalMode = apvts.getRawParameterValue(Params::TraversalMode::id);
        traversalFreq = apvts.getRawParameterValue(Params::TraversalFreq::id);
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
