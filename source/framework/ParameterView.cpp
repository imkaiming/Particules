#include "ParameterView.h"
#include "../utils/PluginParams.h"

ParameterView::ParameterView() : mIsGrainsEmpty{true}, mIsPlaying{false}, mSampleRate{0.0} {}

void ParameterView::init(ValueTreeState& apvts, double sampleRate)
{
    setSampleRate(sampleRate);
    //mSampleRate.store(sampleRate, std::memory_order_relaxed);
    mix = apvts.getRawParameterValue(PluginParams::Mix::id);
    gain = apvts.getRawParameterValue(PluginParams::Gain::id);
    emission = apvts.getRawParameterValue(PluginParams::Emission::id);
    duration = apvts.getRawParameterValue(PluginParams::Duration::id);
    speed = apvts.getRawParameterValue(PluginParams::Speed::id);
    position = apvts.getRawParameterValue(PluginParams::Position::id);
    selection = apvts.getRawParameterValue(PluginParams::Selection::id);
    envMode = apvts.getRawParameterValue(PluginParams::EnvelopeMode::id);
    sustainRatio = apvts.getRawParameterValue(PluginParams::SustainRatio::id);
    traversalMode = apvts.getRawParameterValue(PluginParams::TraversalMode::id);
    traversalFreq = apvts.getRawParameterValue(PluginParams::TraversalFreq::id);

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
