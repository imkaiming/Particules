/*
  ==============================================================================

	Parameters.cpp
	Created: 26 Feb 2023 12:38:46am
	Author:  user

  ==============================================================================
*/

#include "ParameterView.h"

ParameterView::ParameterView() : mIsGrainsEmpty{true}, mIsPlaying{false}, mSampleRate{0.0}
{
    //DBG("DBG -> is sample source lock free : " << (sampleSource.is_lock_free() ? "true" : "false"));
}

void ParameterView::init(ValueTreeState& apvts, double sampleRate)
{
    mSampleRate.store(sampleRate, std::memory_order_relaxed);
    view.mix = apvts.getRawParameterValue(Param::Mix::id);
    view.gain = apvts.getRawParameterValue(Param::Gain::id);
    view.density = apvts.getRawParameterValue(Param::Density::id);
    view.duration = apvts.getRawParameterValue(Param::Duration::id);
    view.speed = apvts.getRawParameterValue(Param::Speed::id);
    view.position = apvts.getRawParameterValue(Param::Position::id);
    view.selection = apvts.getRawParameterValue(Param::Selection::id);
    view.envType = apvts.getRawParameterValue(Param::EnvelopeType::id);
    view.sustainRatio = apvts.getRawParameterValue(Param::SustainRatio::id);
    view.traversalMode = apvts.getRawParameterValue(Param::TraversalMode::id);
    view.traversalTime = apvts.getRawParameterValue(Param::TraversalTime::id);
}

const int ParameterView::getNumChannels() const noexcept
{
    std::shared_ptr<const SampleSource> source = std::atomic_load(&sampleSource);
    return source ? source->numChannels : -1;
    //return sampleSource.load()->numChannels;
}

const int ParameterView::getNumSamples() const noexcept
{
    std::shared_ptr<const SampleSource> source = std::atomic_load(&sampleSource);
    return source ? source->numSamples : -1;
    //return sampleSource.load()->numSamples;
}

const ParameterSnapshot ParameterView::getSnapshot() const noexcept
{
    ParameterSnapshot snapshot;
    snapshot.durationSample = getDuration() * mSampleRate;
    snapshot.startPositionSample = getFilePosition() * mSampleRate;
    snapshot.selectionSample = getWindowSelection() * mSampleRate;
    snapshot.mix = getMix();
    snapshot.speed = getSpeed();
    snapshot.density = getDensity();
    snapshot.envType = getEnvelopeType();
    snapshot.sustainRatio= getSustainRatio();
    snapshot.linearGain = getDecibelToGain();
    snapshot.traversalMode = getTraversalMode();
    snapshot.traversalTime = getTraversalTime();
    snapshot.sampleRate = getSampleRate();
    return snapshot;
};

//void ParameterView::updateGrainVisualizer()
//{
//	grainVisualizer->update();
//}
