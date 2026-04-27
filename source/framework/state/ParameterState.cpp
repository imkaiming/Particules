#include "ParameterState.h"

#include <juce_audio_basics/juce_audio_basics.h> // decibel
#include <juce_audio_processors/juce_audio_processors.h> // apvts

#include "AudioState.h"
#include "framework/core/PluginParams.h"
#include "utils/struct/AudioPayload.h"

namespace particules
{
    //ParameterState::ParameterState(AudioState& as) : audioState{as} {}

    void ParameterState::init(ValueTreeState& apvts)
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
        attack = apvts.getRawParameterValue(params::attack::id);
        decay = apvts.getRawParameterValue(params::decay::id);
        sustain = apvts.getRawParameterValue(params::sustain::id);
        release = apvts.getRawParameterValue(params::release::id);
    }

    EnvelopeMode ParameterState::getEnvelopeMode() const noexcept
    {
        if(!envMode)
            return EnvelopeMode::Hann;

        const float v = envMode ? (envMode->load(std::memory_order_relaxed)) : 0.f;
        const int choice = static_cast<int>(std::round(v));

        if(choice < 0 || choice >= 7)
            return EnvelopeMode::Hann;

        return static_cast<EnvelopeMode>(choice);
    }

    TraversalMode ParameterState::getTraversalMode() const noexcept
    {
        if(!traversalMode)
            return TraversalMode::Sine;

        const float v = traversalMode ? (traversalMode->load(std::memory_order_relaxed)) : 0;
        const int choice = static_cast<int>(std::round(v));
        if(choice <= 0 || choice > 5)
            return TraversalMode::Sine;

        return static_cast<TraversalMode>(choice);
    }

    const ParameterSnapshot ParameterState::getSnapshot(AudioPayload* payload, double sampleRate) noexcept
    {
        ParameterSnapshot ps;

        // state params
        //ps.isAuditionning = getAuditionning() > 0.5f ? true : false;
        ps.playback = getPlayback() > 0.5f ? 1.f : -1.f;
        //// get Buffer data
        // position data
        ps.inputNumSamples = payload->numSamples; // temporary
        ps.startPositionSamples = static_cast<int>(getNormalizedStartPosition() * payload->numSamples);
        ps.spanSamples = static_cast<int>(getNormalizedSpan() * payload->numSamples);

        // time data
        ps.durationSamples = static_cast<int>(getNormalizedDuration() * sampleRate);

        // grain data
        ps.speed = getSpeed();
        ps.emission = getEmission();
        ps.envMode = getEnvelopeMode();
        ps.sustainRatio = getNormalizedSustainRatio();
        ps.linearGain = getLinearGain();
        ps.traversalMode = getTraversalMode();
        ps.traversalFreq = getTraversalFreq();
        ps.attack = getAttack();
        ps.decay = getDecay();
        ps.sustain = getSustain();
        ps.release = getRelease();

        //DBG("attack = " + str(ps.attack));
        //DBG("decay = " + str(ps.decay));
        //DBG("sustain = " + str(ps.sustain));
        //DBG("release = " + str(ps.release));
        return ps;
    };

    float ParameterState::getLinearGain() const noexcept
    {
        return output ? juce::Decibels::decibelsToGain(output->load(std::memory_order_relaxed)) : 0.0f;
    }
}
