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
    view.Emission = apvts.getRawParameterValue(Param::Emission::id);
    view.duration = apvts.getRawParameterValue(Param::Duration::id);
    view.speed = apvts.getRawParameterValue(Param::Speed::id);
    view.position = apvts.getRawParameterValue(Param::Position::id);
    view.selection = apvts.getRawParameterValue(Param::Selection::id);
    view.envType = apvts.getRawParameterValue(Param::EnvelopeType::id);
    view.sustainRatio = apvts.getRawParameterValue(Param::SustainRatio::id);
    view.traversalMode = apvts.getRawParameterValue(Param::TraversalMode::id);
    view.traversalFreq = apvts.getRawParameterValue(Param::TraversalFreq::id);
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
    snapshot.durationSamples = static_cast<int>(getDuration() * mSampleRate);
    snapshot.startPositionSamples = static_cast<int>(getFilePosition() * mSampleRate);
    snapshot.selectionSamples = static_cast<int>(getWindowSelection() * mSampleRate);

    snapshot.mix = getMix();
    snapshot.speed = getSpeed();
    snapshot.emission = getEmission();
    snapshot.envType = getEnvelopeType();
    snapshot.sustainRatio = getSustainRatio();
    snapshot.linearGain = getLinearGain();
    snapshot.traversalMode = static_cast<int>(getTraversalMode());
    snapshot.traversalFreq = getTraversalFreq();
    snapshot.sampleRate = getSampleRate();
    snapshot.inputNumSamples = getNumSamples();
    snapshot.inputNumChannels = getNumChannels();
    return snapshot;
};

void ParameterView::setAudioSource(std::shared_ptr<const AudioBuffer> ib) noexcept
{
    setNumChannels(ib.get()->getNumChannels());
    setNumSamples(ib.get()->getNumSamples());
    inputBuffer.store(std::move(ib), std::memory_order_relaxed);
}

std::shared_ptr<const AudioBuffer> ParameterView::getAudioSource() const noexcept
{
    return inputBuffer.load(std::memory_order_acquire);
}

//void ParameterView::updateGrainVisualizer()
//{
//	grainVisualizer->update();
//}
