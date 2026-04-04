#include "GuiAppProcessor.h"
#include "GuiAppEditor.h"

#include "../utils/PluginParams.h"

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
            adsr::attack::id, adsr::attack::name, juce::NormalisableRange<float>(adsr::attack::min, adsr::attack::max, 0.01f),
            adsr::attack::init, juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            adsr::decay::id, adsr::decay::name, juce::NormalisableRange<float>(adsr::decay::min, adsr::decay::max, 0.01f),
            adsr::decay::init, juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            adsr::sustain::id, adsr::sustain::name, juce::NormalisableRange<float>(adsr::sustain::min, adsr::sustain::max, 0.01f),
            adsr::sustain::init, juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            adsr::release::id, adsr::release::name, juce::NormalisableRange<float>(adsr::release::min, adsr::release::max, 0.01f),
            adsr::release::init, juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        // GLOBAL //

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            global::output::id, global::output::name,
            juce::NormalisableRange<float>(global::output::min, global::output::max, 0.01f), global::output::init,
            juce::String(" dB"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " dB"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            grains::emission::id, grains::emission::name,
            juce::NormalisableRange<float>(grains::emission::min, grains::emission::max, 0.001f), grains::emission::init,
            juce::String(" g/s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " g/s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            grains::duration::id, grains::duration::name,
            juce::NormalisableRange<float>(grains::duration::min, grains::duration::max, 0.001f), grains::duration::init,
            juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(grains::speed::id, grains::speed::name,
            juce::NormalisableRange<float>(grains::speed::min, grains::speed::max, 0.001f), grains::speed::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(global::position::id, global::position::name,
            juce::NormalisableRange<float>(global::position::min, global::position::max, 0.001f), global::position::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(global::span::id, global::span::name,
            juce::NormalisableRange<float>(global::span::min, global::span::max, 0.001f), global::span::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(grains::sustainRatio::id, grains::sustainRatio::name,
            juce::NormalisableRange<float>(grains::sustainRatio::min, grains::sustainRatio::max, 0.01f),
            grains::sustainRatio::init));

        // ENVELOPE MODE //
        const juce::StringArray choicesEnvModeNames(
            grains::envelopeMode::envModeNames.data(), (int)grains::envelopeMode::envModeNames.size());

        layout.add(std::make_unique<juce::AudioParameterChoice>(
            grains::envelopeMode::id, grains::envelopeMode::name, choicesEnvModeNames, 1));

        // TRAVERSAL MODE //

        const juce::StringArray choicesTraversalModeNames(
            grains::traversalMode::traversalModeNames.data(), (int)grains::traversalMode::traversalModeNames.size());

        layout.add(std::make_unique<juce::AudioParameterChoice>(
            grains::traversalMode::id, grains::traversalMode::name, choicesTraversalModeNames, 1));

        // TRAVERSAL FREQ //

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            grains::traversalFreq::id, grains::traversalFreq::name,
            juce::NormalisableRange<float>(grains::traversalFreq::min, grains::traversalFreq::max, 0.01f),
            grains::traversalFreq::init, juce::String(" Hz"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " Hz"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        // ajouter pan, direction
        // randomEMISSION, randomDuration, randomPan, randomDirection, randomPitch

        return layout;
    }
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new particules::GuiAppProcessor(); }