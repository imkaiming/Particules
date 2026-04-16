#include "ParameterView.h"

#include <juce_audio_basics/juce_audio_basics.h> // decibel
#include <juce_audio_processors/juce_audio_processors.h> // apvts

#include "../PluginParams.h"
#include "AudioState.h"

namespace particules
{
    ParameterView::ParameterView(AudioState& as) : audioState{as} {}

    void ParameterView::init(ValueTreeState& apvts)
    {
        //mix = apvts.getRawParameterValue(params::mix::id);
        output = apvts.getRawParameterValue(params::output::id);
        emission = apvts.getRawParameterValue(params::emission::id);
        duration = apvts.getRawParameterValue(params::duration::id);
        speed = apvts.getRawParameterValue(params::speed::id);
        position = apvts.getRawParameterValue(params::position::id);
        span = apvts.getRawParameterValue(params::span::id);
        envMode = apvts.getRawParameterValue(params::envelopeMode::id);
        sustainRatio = apvts.getRawParameterValue(params::envelopeRatio::id);
        traversalMode = apvts.getRawParameterValue(params::traversalMode::id);
        traversalFreq = apvts.getRawParameterValue(params::traversalFreq::id);
        play = apvts.getRawParameterValue(params::play::id);
        playback = apvts.getRawParameterValue(params::playback::id);
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
        ParameterSnapshot ps;
        AudioStateSnapshot es = audioState.getSnapshot();

        // state params
        ps.play = getPlay() > 0.5f ? true : false;
        ps.playback = getPlayback() > 0.5f ? 1.0f : 0.0f;
        //// get Buffer data

        // position data
        ps.inputNumSamples = es.inputNumSamples; // temporary
        ps.startPositionSamples = static_cast<int>(getNormalizedStartPosition() * es.inputNumSamples);
        ps.selectionSamples = static_cast<int>(getNormalizedWindowSelection() * es.inputNumSamples);

        // time data
        ps.durationSamples = static_cast<int>(getNormalizedDuration() * es.sampleRate);

        // grain data
        ps.speed = getSpeed();
        ps.emission = getEmission();
        ps.envMode = getEnvelopeMode();
        ps.sustainRatio = getNormalizedSustainRatio();
        ps.linearGain = getLinearGain();
        ps.traversalMode = getTraversalMode();
        ps.traversalFreq = getTraversalFreq();

        return ps;
    };

    float ParameterView::getLinearGain() const noexcept
    {
        return output ? juce::Decibels::decibelsToGain(output->load(std::memory_order_relaxed)) : 0.0f;
    }
}
