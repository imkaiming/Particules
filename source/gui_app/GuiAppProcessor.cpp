#include "GuiAppProcessor.h"
#include "GuiAppEditor.h"

#include "framework/core/PluginParams.h"
#include "utils/ParamHelpers.h"

namespace particules
{
    GuiAppProcessor::GuiAppProcessor()
        : apvts(*this, nullptr, "Parameters", createParameterLayout()), paramState{audioState}, audioState{}, uiState{},
          uic{apvts, paramState, audioState, uiState, fui, faudio}
    {
        uic.fui.onLoadFile = [this] { DBG("fui load file"); };
        uic.fui.onLoadFilePath = [this](const str& path) { DBG("fui load file path = " + path); };
        uic.fui.onSetPlaying = [this](bool play) { DBG("on set playing " + str(play ? "1.0f" : "0.0f")); };
        uic.fui.onIsPlaying = [this]() -> float { return paramState.getPlay() > 0.5f ? 1.0f : 0.f; };
    }
    void GuiAppProcessor::prepareToPlay(double sampleRate, int /* samplesPerBlock */)
    {
        //const int numChannels = getTotalNumOutputChannels();
        paramState.init(apvts);
        audioState.setSampleRate(sampleRate);
        uiState.init(nullptr);
        //loader.init(sampleRate, numChannels);
    }
    void GuiAppProcessor::releaseResources() {}
    bool GuiAppProcessor::isBusesLayoutSupported(const BusesLayout& /* layouts */) const { return false; }
    void GuiAppProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) { buffer.clear(); }
    juce::AudioProcessorEditor* GuiAppProcessor::createEditor() { return new GuiAppEditor(*this); }
    bool GuiAppProcessor::hasEditor() const { return false; }
    const str GuiAppProcessor::getName() const { return "GuiAppProcessor"; }
    bool GuiAppProcessor::acceptsMidi() const { return false; }
    bool GuiAppProcessor::producesMidi() const { return false; }
    bool GuiAppProcessor::isMidiEffect() const { return false; }
    double GuiAppProcessor::getTailLengthSeconds() const { return 0.0; }
    int GuiAppProcessor::getNumPrograms() { return 1; }
    int GuiAppProcessor::getCurrentProgram() { return 0; }
    void GuiAppProcessor::setCurrentProgram(int /* index */) {}
    const str GuiAppProcessor::getProgramName(int /* index */) { return {}; }
    void GuiAppProcessor::changeProgramName(int /* index*/, const str& /* newName */) {}
    void GuiAppProcessor::getStateInformation(juce::MemoryBlock& /* destData */) {}
    void GuiAppProcessor::setStateInformation(const void* /*data*/, int /*sizeInBytes*/) {}

    ValueTreeState::ParameterLayout GuiAppProcessor::createParameterLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        // STATE //

        layout.add(std::make_unique<juce::AudioParameterBool>(params::play::id, params::play::name, params::play::init));

        // ADSR //

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::attack::id, params::attack::name,
            juce::NormalisableRange<float>(params::attack::min, params::attack::max, 0.01f), params::attack::init, str(" s"),
            juce::AudioProcessorParameter::genericParameter, [](float v, int) { return str(v, 1) + "s"; },
            [](const str& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::decay::id, params::decay::name, juce::NormalisableRange<float>(params::decay::min, params::decay::max, 0.01f),
            params::decay::init, str(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return str(v, 1) + "s"; }, [](const str& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::sustain::id, params::sustain::name,
            juce::NormalisableRange<float>(params::sustain::min, params::sustain::max, 0.01f), params::sustain::init, str(" s"),
            juce::AudioProcessorParameter::genericParameter, [](float v, int) { return str(v, 1) + "s"; },
            [](const str& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::release::id, params::release::name,
            juce::NormalisableRange<float>(params::release::min, params::release::max, 0.01f), params::release::init, str(" s"),
            juce::AudioProcessorParameter::genericParameter, [](float v, int) { return str(v, 1) + "s"; },
            [](const str& s) { return s.getFloatValue(); }));

        // OUTPUT //

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::output::id, params::output::name,
            juce::NormalisableRange<float>(params::output::min, params::output::max, 0.01f), params::output::init, str(" dB"),
            juce::AudioProcessorParameter::genericParameter, [](float v, int) { return str(v, 2) + " dB"; },
            [](const str& s) { return s.getFloatValue(); }));

        // EMISSION //

        layout.add(createNormalizedParameter(
            params::emission::id, params::emission::name, params::emission::min, params::emission::max,
            params::emission::skewFactor, params::emission::init,
            [](float v, int) -> str { return str(v, (v > 1.f ? 2 : (v > 0.101f ? 3 : 4))) + " g/s"; },
            [](const str& s) -> float { return s.getFloatValue(); }));

        layout.add(
            createNormalizedParameter(params::emission_jitter::id, params::emission_jitter::name, params::emission_jitter::min,
                params::emission_jitter::max, params::emission_jitter::skewFactor, params::emission_jitter::init));

        // DURATION //

        layout.add(createNormalizedParameter(
            params::duration::id, params::duration::name, params::duration::min, params::duration::max,
            params::duration::skewFactor, params::duration::init,
            [](float v, int) -> str {
                if(v > 1.0f)
                    return str(v, 2) + " s";
                if(v >= 0.2f)
                    return str(v * 1000.0f, 1) + " ms";
                return str(v * 1000.0f, 2) + " ms";
            },
            [](const str& s) -> float { return s.getFloatValue(); }));

        // SPEED //

        layout.add(createNormalizedParameter(
            params::speed::id, params::speed::name, params::speed::min, params::speed::max, params::speed::skewFactor,
            params::speed::init, [](float v, int) -> str { return ""; },
            [](const str& s) -> float { return s.getFloatValue(); }));

        layout.add(createNormalizedParameter(params::speed_jitter::id, params::speed_jitter::name, params::speed_jitter::min,
            params::speed_jitter::max, params::speed_jitter::skewFactor, params::speed_jitter::init));

        //layout.add(std::make_unique<juce::AudioParameterFloat>(params::speed::id, params::speed::name,
        //    juce::NormalisableRange<float>(params::speed::min, params::speed::max, 0.001f), params::speed::init));

        // PAN //

        layout.add(createNormalizedParameter(
            params::pan::id, params::pan::name, params::pan::min, params::pan::max, params::pan::skewFactor, params::pan::init,
            [](float v, int) -> str { return ""; }, [](const str& s) -> float { return s.getFloatValue(); }));

        layout.add(createNormalizedParameter(params::pan_jitter::id, params::pan_jitter::name, params::pan_jitter::min,
            params::pan_jitter::max, params::pan_jitter::skewFactor, params::pan_jitter::init));

        //layout.add(std::make_unique<juce::AudioParameterFloat>(params::pan::id, params::pan::name,
        //    juce::NormalisableRange<float>(params::pan::min, params::pan::max, 0.001f), params::pan::init));

        // POSITION //

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::position::id, params::position::name,
            juce::NormalisableRange<float>(params::position::min, params::position::max, 0.001f), params::position::init));

        // SPAN //

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::span::id, params::span::name,
            juce::NormalisableRange<float>(params::span::min, params::span::max, 0.001f), params::span::init));

        // ENVELOPE RATIO

        juce::NormalisableRange<float> envelopeRatioRange{params::envelopeRatio::min, params::envelopeRatio::max}; //, 0.01f};
        envelopeRatioRange.setSkewForCentre(params::envelopeRatio::skewFactor);

        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{params::envelopeRatio::id, 1},
            params::envelopeRatio::name, envelopeRatioRange, params::envelopeRatio::init,
            juce::AudioParameterFloatAttributes{}
                .withCategory(juce::AudioProcessorParameter::genericParameter)
                .withStringFromValueFunction([](float v, int) { return str(v, (v > 0.1f ? 2 : 3)); })
                .withValueFromStringFunction([](const str& s) { return s.getFloatValue(); })));

        layout.add(createNormalizedParameter(params::envelopeRatio_jitter::id, params::envelopeRatio_jitter::name,
            params::envelopeRatio_jitter::min, params::envelopeRatio_jitter::max, params::envelopeRatio_jitter::skewFactor,
            params::envelopeRatio_jitter::init));

        // ENVELOPE MODE //

        const juce::StringArray choicesEnvModeNames(
            params::envelopeMode::envModeNames.data(), (int)params::envelopeMode::envModeNames.size());

        layout.add(std::make_unique<juce::AudioParameterChoice>(
            params::envelopeMode::id, params::envelopeMode::name, choicesEnvModeNames, 1));

        // TRAVERSAL MODE //

        const juce::StringArray choicesTraversalModeNames(
            params::traversalMode::traversalModeNames.data(), (int)params::traversalMode::traversalModeNames.size());

        layout.add(std::make_unique<juce::AudioParameterChoice>(
            params::traversalMode::id, params::traversalMode::name, choicesTraversalModeNames, 1));

        // TRAVERSAL FREQ //

        juce::NormalisableRange<float> traversalFreqRange{params::traversalFreq::min, params::traversalFreq::max}; //, 0.01f};
        traversalFreqRange.setSkewForCentre(params::traversalFreq::skewFactor);

        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{params::traversalFreq::id, 1},
            params::traversalFreq::name, traversalFreqRange, params::traversalFreq::init,
            juce::AudioParameterFloatAttributes{}
                .withCategory(juce::AudioProcessorParameter::genericParameter)
                .withStringFromValueFunction([](float v, int) { return str(v, (v > 0.1f ? 2 : 3)) + " Hz"; })
                .withValueFromStringFunction([](const str& s) { return s.getFloatValue(); })));

        layout.add(createNormalizedParameter(params::traversalFreq_jitter::id, params::traversalFreq_jitter::name,
            params::traversalFreq_jitter::min, params::traversalFreq_jitter::max, params::traversalFreq_jitter::skewFactor,
            params::traversalFreq_jitter::init));

        // PLAYBACK

        layout.add(createNormalizedParameter(params::playback::id, params::playback::name, params::playback::min,
            params::playback::max, params::playback::skewFactor, params::playback::init));

        return layout;
    }
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new particules::GuiAppProcessor(); }