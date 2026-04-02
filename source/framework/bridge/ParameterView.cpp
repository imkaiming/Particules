#include "ParameterView.h"
#include "../../utils/PluginParams.h"

namespace particules
{
    ParameterView::ParameterView(EngineState& es) : engineState{es} {}

    void ParameterView::init(ValueTreeState& apvts)
    {
        //mix = apvts.getRawParameterValue(global::mix::id);
        output = apvts.getRawParameterValue(global::output::id);
        emission = apvts.getRawParameterValue(grains::emission::id);
        duration = apvts.getRawParameterValue(grains::duration::id);
        speed = apvts.getRawParameterValue(grains::speed::id);
        position = apvts.getRawParameterValue(global::position::id);
        span = apvts.getRawParameterValue(global::span::id);
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

    ParameterSnapshot ParameterView::getSnapshot() const noexcept
    {
        ParameterSnapshot ps;
        EngineSnapshot es = engineState.getSnapshot();

        ps.play = getPlay() > 0.5f ? true : false;
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
}
