#include "GuiAppProcessor.h"
#include "GuiAppEditor.h"

#include "../framework/PluginParams.h"

namespace particules
{
    GuiAppProcessor::GuiAppProcessor()
        : apvts(*this, nullptr, "Parameters", createParameterLayout()), paramsView{engineState}, engineState{}, uiState{},
          uic{apvts, paramsView, engineState, uiState, facade} //, loader{}
    {
        uic.facade.loadFile = [this] { DBG("facade load file"); };
        uic.facade.loadFilePath = [this](const str& path) { DBG("facade load file path = " + path); };
    }

    void GuiAppProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        const int numChannels = getTotalNumOutputChannels();
        paramsView.init(apvts);
        engineState.setSampleRate(sampleRate);
        uiState.init(&visualBuffer);
        //loader.init(sampleRate, numChannels);
    }
    void GuiAppProcessor::releaseResources() {}
    bool GuiAppProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const { return false; }
    void GuiAppProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) { buffer.clear(); }
    juce::AudioProcessorEditor* GuiAppProcessor::createEditor() { return new GuiAppEditor(*this); }
    bool GuiAppProcessor::hasEditor() const { return false; }
    const juce::String GuiAppProcessor::getName() const { return "GuiAppProcessor"; }
    bool GuiAppProcessor::acceptsMidi() const { return false; }
    bool GuiAppProcessor::producesMidi() const { return false; }
    bool GuiAppProcessor::isMidiEffect() const { return false; }
    double GuiAppProcessor::getTailLengthSeconds() const { return 0.0; }
    int GuiAppProcessor::getNumPrograms() { return 1; }
    int GuiAppProcessor::getCurrentProgram() { return 0; }
    void GuiAppProcessor::setCurrentProgram(int index) {}
    const str GuiAppProcessor::getProgramName(int index) { return {}; }
    void GuiAppProcessor::changeProgramName(int index, const str& newName) {}
    void GuiAppProcessor::getStateInformation(juce::MemoryBlock& destData) {}
    void GuiAppProcessor::setStateInformation(const void* data, int sizeInBytes) {}

    //void GuiAppProcessor::initOnAudioLoadedCallback() {
    //    onAudioLoadedCallback =
    //        [&]() {
    //            DBG("onAudioLoadedCallback");
    //            //setInputBuffer(buffer);
    //            //const juce::File& f = getCurrentFile();
    //            //if(f.existsAsFile())
    //            //{
    //            //audioThumbnail.setSource(new juce::FileInputSource(f));
    //            //engineState.setNumSamples(buffer.getNumSamples());
    //            //engineState.setNumChannels(buffer.getNumChannels());
    //            //uiState.setNumSamples(buffer.getNumSamples());
    //            //uiState.setFileLoaded(true);
    //        }
    //};

    //void GuiAppProcessor::loadFile(const str& path) { loader.loadFile(path, onAudioLoadedCallback); }

    //void GuiAppProcessor::loadFile() { loader.loadFile(onAudioLoadedCallback); }

    //ValueTreeState::ParameterLayout createParameterLayout() { return ValueTreeState::ParameterLayout(); }
    ValueTreeState::ParameterLayout GuiAppProcessor::createParameterLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        // ADSR //
        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::attack::id, params::attack::name, juce::NormalisableRange<float>(params::attack::min, params::attack::max, 0.01f),
            params::attack::init, juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::decay::id, params::decay::name, juce::NormalisableRange<float>(params::decay::min, params::decay::max, 0.01f),
            params::decay::init, juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::sustain::id, params::sustain::name, juce::NormalisableRange<float>(params::sustain::min, params::sustain::max, 0.01f),
            params::sustain::init, juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::release::id, params::release::name, juce::NormalisableRange<float>(params::release::min, params::release::max, 0.01f),
            params::release::init, juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        // GLOBAL //

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::output::id, params::output::name,
            juce::NormalisableRange<float>(params::output::min, params::output::max, 0.01f), params::output::init,
            juce::String(" dB"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " dB"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::emission::id, params::emission::name,
            juce::NormalisableRange<float>(params::emission::min, params::emission::max, 0.001f), params::emission::init,
            juce::String(" g/s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " g/s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::duration::id, params::duration::name,
            juce::NormalisableRange<float>(params::duration::min, params::duration::max, 0.001f), params::duration::init,
            juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::speed::id, params::speed::name,
            juce::NormalisableRange<float>(params::speed::min, params::speed::max, 0.001f), params::speed::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::position::id, params::position::name,
            juce::NormalisableRange<float>(params::position::min, params::position::max, 0.001f), params::position::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::span::id, params::span::name,
            juce::NormalisableRange<float>(params::span::min, params::span::max, 0.001f), params::span::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::sustainRatio::id, params::sustainRatio::name,
            juce::NormalisableRange<float>(params::sustainRatio::min, params::sustainRatio::max, 0.01f),
            params::sustainRatio::init));

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

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            params::traversalFreq::id, params::traversalFreq::name,
            juce::NormalisableRange<float>(params::traversalFreq::min, params::traversalFreq::max, 0.01f),
            params::traversalFreq::init, juce::String(" Hz"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " Hz"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        // ajouter pan, direction
        // randomEMISSION, randomDuration, randomPan, randomDirection, randomPitch

        return layout;
    }
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new particules::GuiAppProcessor(); }