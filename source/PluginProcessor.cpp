#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "utils/PluginParams.h"
#include "utils/struct/ParameterSnapshot.h"

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
          apvts(*this, nullptr, "Parameters", createParameterLayout()), //apvts stands for audio processor value tree state
          paramsView(), granularEngine(visualBuffer),
          uiContext{apvts, paramsView, *this, visualBuffer, audioThumbnail}, loader{},
          debugPresetLoaded{false}, cache{5}, audioThumbnail{samplesPerThumbnail, loader.getFormatManager(), cache}
#endif
    {
        //DBG("PLUGIN CTOR");
        initOnAudioLoadedCallback();
    }

    ParticulesAudioProcessor::~ParticulesAudioProcessor() {}

    const juce::String ParticulesAudioProcessor::getName() const { return JucePlugin_Name; }

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

    void ParticulesAudioProcessor::setCurrentProgram(int index) {}

    const juce::String ParticulesAudioProcessor::getProgramName(int index) { return {}; }

    void ParticulesAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

    void ParticulesAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        //DBG("PREPARE TO PLAY");
        const int numChannels = getTotalNumOutputChannels();
        paramsView.init(apvts, sampleRate);
        granularEngine.init(sampleRate, numChannels, samplesPerBlock);
        loader.init(sampleRate, numChannels);
    }

    void ParticulesAudioProcessor::releaseResources() {}

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

    void ParticulesAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
    {
        const ParameterSnapshot snapshot = paramsView.getSnapshot();
        if(!snapshot.isValid())
            return;

        const int inputuNumChannels = getTotalNumInputChannels();
        const int outputNumChannels = getTotalNumOutputChannels();
        const int bufferSize = buffer.getNumSamples();
        float* const* outputPtrs = buffer.getArrayOfWritePointers();

        for(int i = inputuNumChannels; i < outputNumChannels; ++i)
            juce::FloatVectorOperations::clear(outputPtrs[i], bufferSize);

        if(paramsView.getIsPlaying() || !paramsView.getIsGrainsEmpty())
            granularEngine.process(buffer, bufferSize, outputPtrs, outputNumChannels, snapshot);
    }

    bool ParticulesAudioProcessor::hasEditor() const
    {
        return true; // (change this to false if you choose to not supply an editor)
    }

    juce::AudioProcessorEditor* ParticulesAudioProcessor::createEditor()
    {
        ParticulesAudioProcessorEditor* editor = new ParticulesAudioProcessorEditor(*this);
        return editor;
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
            runtimeParametersNode.setProperty("isPlaying", paramsView.getIsPlaying(), nullptr);
            runtimeParametersNode.setProperty("numSamples", paramsView.getNumSamples(), nullptr);
            runtimeParametersNode.setProperty("sampleRate", paramsView.getSampleRate(), nullptr);
            runtimeParametersNode.setProperty("numChannels", paramsView.getNumChannels(), nullptr);
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
                paramsView.setIsPlaying(runtimeParametersNode.getProperty("isPlaying"));
                paramsView.setIsPlaying(runtimeParametersNode.getProperty("numChannels"));
                paramsView.setIsPlaying(runtimeParametersNode.getProperty("sampleRate"));
                paramsView.setIsPlaying(runtimeParametersNode.getProperty("numSamples"));
                loader.setSampleRate(paramsView.getSampleRate());
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

        //juce::Logger::outputDebugString("apvts mix : " + (juce::String)apvts.getRawParameterValue(MIX_ID)->load());
        //juce::Logger::outputDebugString("apvts gain : " + (juce::String)apvts.getRawParameterValue(GAIN_ID)->load());
        //juce::Logger::outputDebugString("apvts dens : " + (juce::String)apvts.getRawParameterValue(EMISSION_ID)->load());
        //juce::Logger::outputDebugString("apvts dur : " + (juce::String)apvts.getRawParameterValue(DURATION_ID)->load());
        //juce::Logger::outputDebugString("apvts pitch : " + (juce::String)apvts.getRawParameterValue(PITCH_ID)->load());
    }

    juce::AudioProcessorValueTreeState::ParameterLayout ParticulesAudioProcessor::createParameterLayout()
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
            global::mix::id, global::mix::name, juce::NormalisableRange<float>(global::mix::min, global::mix::max, 0.01f),
            global::mix::init, juce::String(" %"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "%"; }, [](const juce::String& s) { return s.getFloatValue(); }));

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

        layout.add(std::make_unique<juce::AudioParameterFloat>(global::selection::id, global::selection::name,
            juce::NormalisableRange<float>(global::selection::min, global::selection::max, 0.001f), global::selection::init));

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

    void ParticulesAudioProcessor::setInputBuffer(AudioBuffer& buffer) noexcept
    {
        DBG("SET INPUT BUFFER");
        const int inputChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        paramsView.setNumChannels(inputChannels);
        paramsView.setNumSamples(numSamples);

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

    void ParticulesAudioProcessor::initOnAudioLoadedCallback()
    {
        onAudioLoadedCallback = [this](AudioBuffer& buffer) {
            setInputBuffer(buffer);
            const juce::File& f = getCurrentFile();
            if(f.existsAsFile())
            {
                audioThumbnail.setSource(new juce::FileInputSource(f));
                sendChangeMessage(); // this is a message to the AudioFIlePanel stating "a new file has been succesfully loaded"
            }
        };
    }

    void ParticulesAudioProcessor::loadFile(const juce::String& path) { loader.loadFile(path, onAudioLoadedCallback); }

    void ParticulesAudioProcessor::loadFile() { loader.loadFile(onAudioLoadedCallback); }

    void ParticulesAudioProcessor::loadDebugPreset()
    {
        //DBG("SAMPLE RATE = " + (juce::String) paramsView.getSampleRate());
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

        juce::NormalisableRange<float> gainRange(global::output::min, global::output::max);
        const float normalizedGain = gainRange.convertTo0to1(global::output::init);

        juce::NormalisableRange<float> EmissionRange(grains::emission::min, grains::emission::max);
        const float normalizedEmission = EmissionRange.convertTo0to1(10);

        juce::NormalisableRange<float> durationRange(grains::duration::min, grains::duration::max);
        const float normalizedDuration = durationRange.convertTo0to1(0.05f);

        juce::NormalisableRange<float> speedRange(grains::speed::min, grains::speed::max);
        const float normalizedSpeed = speedRange.convertTo0to1(1);

        juce::NormalisableRange<float> positionRange(global::position::min, global::position::max);
        const float normalizedPosition = positionRange.convertTo0to1(0);

        juce::NormalisableRange<float> selectionRange(global::selection::min, global::selection::max);
        const float normalizedSelection = selectionRange.convertTo0to1(0.25f);

        juce::NormalisableRange<float> TraversalFreqRange(grains::traversalFreq::min, grains::traversalFreq::max);
        const float normalizedTraversalFreq = TraversalFreqRange.convertTo0to1(1.f);

        juce::NormalisableRange<float> SustainRatioRange(grains::sustainRatio::min, grains::sustainRatio::max);
        const float normalizedSustainRatio = SustainRatioRange.convertTo0to1(0.5f);

        apvts.getParameter(global::mix::id)->setValueNotifyingHost(1.f); // MIX100%
        apvts.getParameter(global::output::id)->setValueNotifyingHost(normalizedGain);
        apvts.getParameter(grains::emission::id)->setValueNotifyingHost(normalizedEmission);
        apvts.getParameter(grains::duration::id)->setValueNotifyingHost(normalizedDuration);
        apvts.getParameter(grains::speed::id)->setValueNotifyingHost(normalizedSpeed);
        apvts.getParameter(global::position::id)->setValueNotifyingHost(normalizedPosition);
        apvts.getParameter(global::selection::id)->setValueNotifyingHost(normalizedSelection);
        apvts.getParameter(grains::traversalFreq::id)->setValueNotifyingHost(normalizedTraversalFreq);
        apvts.getParameter(grains::sustainRatio::id)->setValueNotifyingHost(normalizedSustainRatio);
        apvts.getParameter(grains::envelopeMode::id)->setValueNotifyingHost(0.f);
        apvts.getParameter(grains::traversalMode::id)->setValueNotifyingHost(0.f);

        if(debugAudio.existsAsFile())
            loadFile(debugAudio.getFullPathName());

        //if(paramsView.getSampleSource())
        //	DBG("SAMPLESOURCE OK");

        //apvts.getParameter(Param::EnvelopeWidth::id)->setValueNotifyingHost(100.f);
        //apvts.getParameter(Param::TraversalMode::id)->;
        //apvts.getParameter(Param::TraversalFreq::id)->;

        debugPresetLoaded = true;
    }

}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new particules::ParticulesAudioProcessor(); }
