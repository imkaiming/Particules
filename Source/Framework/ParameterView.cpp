/*
  ==============================================================================

	Parameters.cpp
	Created: 26 Feb 2023 12:38:46am
	Author:  user

  ==============================================================================
*/

#include "ParameterView.h"
#include "PluginParams.h"

ParameterView::ParameterView() : mIsGrainsEmpty{true}, mIsPlaying{false}, mSampleRate{0.0} {}

void ParameterView::init(ValueTreeState& apvts, double sampleRate)
{
    setSampleRate(sampleRate);
    //mSampleRate.store(sampleRate, std::memory_order_relaxed);
    view.mix = apvts.getRawParameterValue(Param::Mix::id);
    view.gain = apvts.getRawParameterValue(Param::Gain::id);
    view.emission = apvts.getRawParameterValue(Param::Emission::id);
    view.duration = apvts.getRawParameterValue(Param::Duration::id);
    view.speed = apvts.getRawParameterValue(Param::Speed::id);
    view.position = apvts.getRawParameterValue(Param::Position::id);
    view.selection = apvts.getRawParameterValue(Param::Selection::id);
    view.envMode = apvts.getRawParameterValue(Param::EnvelopeMode::id);
    view.sustainRatio = apvts.getRawParameterValue(Param::SustainRatio::id);
    view.traversalMode = apvts.getRawParameterValue(Param::TraversalMode::id);
    view.traversalFreq = apvts.getRawParameterValue(Param::TraversalFreq::id);
}

EnvelopeMode ParameterView::getEnvelopeMode() const noexcept
{
    if(!view.envMode)
        return EnvelopeMode::Hann;

    const float v = view.envMode ? (view.envMode->load(std::memory_order_relaxed)) : 0.f;
    const int choice = static_cast<int>(std::round(v));

    if(choice < 0 || choice >= 7)
        return EnvelopeMode::Hann;

    return static_cast<EnvelopeMode>(choice);
}

TraversalMode ParameterView::getTraversalMode() const noexcept
{
    if(!view.traversalMode)
        return TraversalMode::Sine;

    const float v = view.traversalMode ? (view.traversalMode->load(std::memory_order_relaxed)) : 0;
    const int choice = static_cast<int>(std::round(v));
    if(choice <= 0 || choice > 5)
        return TraversalMode::Sine;

    return static_cast<TraversalMode>(choice);
}

//const int ParameterView::getNumChannels() const noexcept
//{
//    std::shared_ptr<const AudioBuffer> source = std::atomic_load(&inputBuffer);
//    return source ? source->getNumChannels() : -1;
//    //return sampleSource.load()->numChannels;
//}
//
//const int ParameterView::getNumSamples() const noexcept
//{
//    std::shared_ptr<const AudioBuffer> source = std::atomic_load(&inputBuffer);
//    return source ? source->getNumSamples() : -1;
//    //return sampleSource.load()->numSamples;
//}

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

//void ParameterView::setAudioSource(std::shared_ptr<const AudioBuffer> ib) noexcept
//{
//    setNumChannels(ib.get()->getNumChannels());
//    setNumSamples(ib.get()->getNumSamples());
//    inputBuffer.store(std::move(ib), std::memory_order_relaxed);
//}
//
//std::shared_ptr<const AudioBuffer> ParameterView::getAudioSource() const noexcept
//{
//    return inputBuffer.load(std::memory_order_acquire);
//}
