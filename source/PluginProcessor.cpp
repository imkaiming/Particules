#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <juce_audio_processors_headless/juce_audio_processors_headless.h>

#include "framework/PluginParams.h"
#include "utils/struct/ParameterSnapshot.h"
#include "utils/struct/ProcessorFacade.h"

namespace particules
{
    ParticulesAudioProcessor::ParticulesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
        : AudioProcessor(BusesProperties()
    #if !JucePlugin_IsMidiEffect
        #if !JucePlugin_IsSynth
                  .withInput("Input", juce::AudioChannelSet::stereo(), true)
        #endif
                  .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    #endif
                  ),
          apvts(*this, nullptr, "Parameters", createParameterLayout()), paramsView{engineState},
          granularEngine{visualBuffer, engineState}, engineState{}, uiState{},
          uic{apvts, paramsView, engineState, uiState, facade}, loader{}, debugPresetLoaded{false}
#endif
    {
        onAudioLoadedCallback = [this](AudioBuffer& buffer) {
            setInputBuffer(buffer);
            const juce::File& f = getCurrentFile();
            if(f.existsAsFile())
            {
                engineState.setNumSamples(buffer.getNumSamples());
                engineState.setNumChannels(buffer.getNumChannels());
                uiState.setNumSamples(buffer.getNumSamples());
                uiState.setSource(f);
            }
        };

        facade.loadFile = [this] { loadFile(); };
        facade.loadFilePath = [this](const str& path) { loadFile(path); };
    }

    ParticulesAudioProcessor::~ParticulesAudioProcessor() {}

    void ParticulesAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        //DBG("PREPARE TO PLAY");
        const int numChannels = getTotalNumOutputChannels();
        paramsView.init(apvts);
        engineState.setSampleRate(sampleRate);
        uiState.init(&visualBuffer);
        granularEngine.init(sampleRate, numChannels, samplesPerBlock);
        loader.init(sampleRate, numChannels);
    }

    void ParticulesAudioProcessor::releaseResources() {}

    void ParticulesAudioProcessor::processBlock(AudioBuffer& buffer, juce::MidiBuffer& midiMessages)
    {
        ParameterSnapshot ps = paramsView.getSnapshot();

        if(!ps.isValid())
            return;

        const int inputuNumChannels = getTotalNumInputChannels();
        const int outputNumChannels = getTotalNumOutputChannels();
        const int bufferSize = buffer.getNumSamples();
        float* const* outputPtrs = buffer.getArrayOfWritePointers();

        for(int i = inputuNumChannels; i < outputNumChannels; ++i)
            juce::FloatVectorOperations::clear(outputPtrs[i], bufferSize);

        if(ps.play)
            granularEngine.process(buffer, bufferSize, outputPtrs, outputNumChannels, ps);
    }

    void ParticulesAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
    {
        //DBG("GET STATE INFO");
        juce::ValueTree vt = apvts.copyState();
        const juce::File& f = getCurrentFile();
        if(f.existsAsFile())
        {
            // adding the input buffer to the value tree
            //juce::ValueTree audioFileNode("AudioFile");
            //audioFileNode.setProperty("path", f.getFullPathName(), nullptr);
            //vt.appendChild(audioFileNode, nullptr);

            juce::ValueTree runtimeParametersNode("RuntimeParameters");
            //runtimeParametersNode.setProperty("isPlaying", engineState.getIsPlaying(), nullptr);
            runtimeParametersNode.setProperty("numSamples", engineState.getNumSamples(), nullptr);
            runtimeParametersNode.setProperty("sampleRate", engineState.getSampleRate(), nullptr);
            runtimeParametersNode.setProperty("numChannels", engineState.getNumChannels(), nullptr);
            vt.appendChild(runtimeParametersNode, nullptr);
        }

        std::unique_ptr<juce::XmlElement> xml(vt.createXml());
        copyXmlToBinary(*xml, destData);
    }

    void ParticulesAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
    {
        //DBG("SET STATE INFO");
        std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
        if(xmlState == nullptr)
            return;

        // si on trouve un fichier xml alors on met à jours les données dans apvts
        if(xmlState->hasTagName(apvts.state.getType()))
        {
            juce::ValueTree vt = juce::ValueTree::fromXml(*xmlState);

            juce::ValueTree runtimeParametersNode = vt.getChildWithName("RuntimeParameters");
            if(runtimeParametersNode.isValid())
            {
                //engineState.setIsPlaying(runtimeParametersNode.getProperty("isPlaying"));
                engineState.setNumChannels(runtimeParametersNode.getProperty("numChannels"));
                engineState.setSampleRate(runtimeParametersNode.getProperty("sampleRate"));
                engineState.setNumSamples(runtimeParametersNode.getProperty("numSamples"));
                loader.setSampleRate(engineState.getSampleRate());
            }

            //juce::ValueTree audioFileNode = vt.getChildWithName("AudioFile");
            //if(audioFileNode.isValid())
            //{
            //    str path = audioFileNode.getProperty("path");
            //    loadFile(path);
            //}
            //vt.removeChild(audioFileNode, nullptr);

            vt.removeChild(runtimeParametersNode, nullptr);

            apvts.replaceState(vt);
        }
    }

    void ParticulesAudioProcessor::setInputBuffer(AudioBuffer& buffer) noexcept
    {
        DBG("SET INPUT BUFFER");
        const int inputChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        engineState.setNumChannels(inputChannels);
        engineState.setNumSamples(numSamples);

        // adding a guard sample to the input buffer to make it safe to interpolate the buffer's read position
        AudioBuffer tempBuffer(inputChannels, numSamples + 1);

        for(int ch = 0; ch < inputChannels; ch++)
        {
            juce::FloatVectorOperations::copy(tempBuffer.getWritePointer(ch), buffer.getReadPointer(ch), numSamples);
            tempBuffer.getWritePointer(ch)[numSamples] = tempBuffer.getReadPointer(ch)[0];
        }

        std::shared_ptr<const AudioBuffer> safeBufferPtr = std::make_shared<const AudioBuffer>(std::move(tempBuffer));

        granularEngine.setInputBuffer(std::move(safeBufferPtr));
    }

    void ParticulesAudioProcessor::loadFile(const str& path) { loader.loadFile(path, onAudioLoadedCallback); }

    void ParticulesAudioProcessor::loadFile() { loader.loadFile(onAudioLoadedCallback); }

    ValueTreeState::ParameterLayout ParticulesAudioProcessor::createParameterLayout()
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

        layout.add(createSkewedWithOffset(
            params::emission::id, params::emission::name, params::emission::min, params::emission::max,
            params::emission::skewFactor, params::emission::init,
            [](float v, int) -> str { return str(v, (v > 1.f ? 2 : (v > 0.101f ? 3 : 4))) + " g/s"; },
            [](const str& s) -> float { return s.getFloatValue(); }));

        // DURATION //

        layout.add(createSkewedWithOffset(
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

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::speed::id, params::speed::name,
            juce::NormalisableRange<float>(params::speed::min, params::speed::max, 0.001f), params::speed::init));

        // POSITION //

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::position::id, params::position::name,
            juce::NormalisableRange<float>(params::position::min, params::position::max, 0.001f), params::position::init));

        // SPAN //

        layout.add(std::make_unique<juce::AudioParameterFloat>(params::span::id, params::span::name,
            juce::NormalisableRange<float>(params::span::min, params::span::max, 0.001f), params::span::init));

        // ENVELOPE RATION

        juce::NormalisableRange<float> envelopeRatioRange{params::envelopeRatio::min, params::envelopeRatio::max}; //, 0.01f};
        envelopeRatioRange.setSkewForCentre(params::envelopeRatio::skewFactor);

        layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{params::envelopeRatio::id, 1},
            params::envelopeRatio::name, envelopeRatioRange, params::envelopeRatio::init,
            juce::AudioParameterFloatAttributes{}
                .withCategory(juce::AudioProcessorParameter::genericParameter)
                .withStringFromValueFunction([](float v, int) { return str(v, (v > 0.1f ? 2 : 3)); })
                .withValueFromStringFunction([](const str& s) { return s.getFloatValue(); })));

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

        // ajouter pan, direction
        // randomEMISSION, randomDuration, randomPan, randomDirection, randomPitch

        return layout;
    }

    void ParticulesAudioProcessor::loadDebugPreset()
    {
        //DBG("SAMPLE RATE = " + (str) paramsView.getSampleRate());
        DBG("LOAD DBG PRESET");
        const juce::File debugAudio = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
                                          .getParentDirectory()
                                          .getParentDirectory()
                                          .getParentDirectory()
                                          .getParentDirectory()
                                          .getParentDirectory()
                                          .getParentDirectory()
                                          .getParentDirectory()
                                          .getChildFile("resources")
                                          .getChildFile("audio")
                                          .getChildFile("01_Piano_E.wav");

        //DBG("juce::File::currentApplicationFile " + debugAudioPlaceHolder.getFullPathName());

        if(debugPresetLoaded)
            return;

        //juce::File debugFile = juce::File::getSpecialLocation(
        //	juce::File::userDesktopDirectory
        //).getChildFile("test.wav");

        juce::NormalisableRange<float> gainRange(params::output::min, params::output::max);
        const float normalizedGain = gainRange.convertTo0to1(params::output::init);

        juce::NormalisableRange<float> emissionRange(params::emission::min, params::emission::max);
        const float normalizedEmission = emissionRange.convertTo0to1(10);

        juce::NormalisableRange<float> durationRange(params::duration::min, params::duration::max);
        const float normalizedDuration = durationRange.convertTo0to1(0.05f);

        juce::NormalisableRange<float> speedRange(params::speed::min, params::speed::max);
        const float normalizedSpeed = speedRange.convertTo0to1(1);

        juce::NormalisableRange<float> positionRange(params::position::min, params::position::max);
        const float normalizedPosition = positionRange.convertTo0to1(0);

        juce::NormalisableRange<float> spanRange(params::span::min, params::span::max);
        const float normalizedSpan = spanRange.convertTo0to1(0.25f);

        juce::NormalisableRange<float> TraversalFreqRange(params::traversalFreq::min, params::traversalFreq::max);
        const float normalizedTraversalFreq = TraversalFreqRange.convertTo0to1(1.f);

        juce::NormalisableRange<float> SustainRatioRange(params::envelopeRatio::min, params::envelopeRatio::max);
        const float normalizedSustainRatio = SustainRatioRange.convertTo0to1(0.5f);

        //apvts.getParameter(params::mix::id)->setValueNotifyingHost(1.f); // MIX100%
        apvts.getParameter(params::output::id)->setValueNotifyingHost(normalizedGain);
        apvts.getParameter(params::emission::id)->setValueNotifyingHost(normalizedEmission);
        apvts.getParameter(params::duration::id)->setValueNotifyingHost(normalizedDuration);
        apvts.getParameter(params::speed::id)->setValueNotifyingHost(normalizedSpeed);
        apvts.getParameter(params::position::id)->setValueNotifyingHost(normalizedPosition);
        apvts.getParameter(params::span::id)->setValueNotifyingHost(normalizedSpan);
        apvts.getParameter(params::traversalFreq::id)->setValueNotifyingHost(normalizedTraversalFreq);
        apvts.getParameter(params::envelopeRatio::id)->setValueNotifyingHost(normalizedSustainRatio);
        apvts.getParameter(params::envelopeMode::id)->setValueNotifyingHost(0.f);
        apvts.getParameter(params::traversalMode::id)->setValueNotifyingHost(0.f);

        if(debugAudio.existsAsFile())
            loadFile(debugAudio.getFullPathName());

        //if(paramsView.getSampleSource())
        //	DBG("SAMPLESOURCE OK");

        //apvts.getParameter(Param::EnvelopeWidth::id)->setValueNotifyingHost(100.f);
        //apvts.getParameter(Param::TraversalMode::id)->;
        //apvts.getParameter(Param::TraversalFreq::id)->;

        debugPresetLoaded = true;
    }

    juce::AudioProcessorEditor* ParticulesAudioProcessor::createEditor()
    {
        ParticulesAudioProcessorEditor* editor = new ParticulesAudioProcessorEditor(*this);
        return editor;
    }

    const juce::String ParticulesAudioProcessor::getName() const { return JucePlugin_Name; }

#ifndef JucePlugin_PreferredChannelConfigurations
    bool ParticulesAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
    {
    #if JucePlugin_IsMidiEffect
        juce::ignoreUnused(layouts);
        return true;
    #else
        // This is the place where you check if the layout is supported.
        // In this template code we only support mono or stereo.
        // Some plugin hosts, such as certain GarageBand versions, will only
        // load plugins that support stereo bus layouts.
        if(layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
            && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
            return false;

        // This checks if the input layout matches the output layout
        #if !JucePlugin_IsSynth
        if(layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
            return false;
        #endif

        return true;
    #endif
    }
#endif

    bool ParticulesAudioProcessor::acceptsMidi() const
    {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }

    bool ParticulesAudioProcessor::producesMidi() const
    {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

    bool ParticulesAudioProcessor::isMidiEffect() const
    {
#if JucePlugin_IsMidiEffect
        return true;
#else
        return false;
#endif
    }

    double ParticulesAudioProcessor::getTailLengthSeconds() const { return 0.0; }

    int ParticulesAudioProcessor::getNumPrograms()
    {
        return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
    }

    int ParticulesAudioProcessor::getCurrentProgram() { return 0; }

    void ParticulesAudioProcessor::setCurrentProgram(int /*index*/) {}

    const juce::String ParticulesAudioProcessor::getProgramName(int /*index*/) { return {}; }

    void ParticulesAudioProcessor::changeProgramName(int /* index */, const str& /* newName */) {}

    bool ParticulesAudioProcessor::hasEditor() const
    {
        return true; // (change this to false if you choose to not supply an editor)
    }

}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new particules::ParticulesAudioProcessor(); }
