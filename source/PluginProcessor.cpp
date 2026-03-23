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
          paramsView(), granularEngine(visualBuffer), uiContext{apvts, paramsView, customLookAndFeel, *this, visualBuffer},
          loader{}, debugPresetLoaded{false}
#endif
    {
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
        juce::ValueTree vt = apvts.copyState();
        std::unique_ptr<juce::XmlElement> xml(vt.createXml());
        copyXmlToBinary(*xml, destData);
    }

    void ParticulesAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
    {
        std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

        if(xmlState.get() != nullptr)
        {
            // si on trouve un fichier xml alors on met à jours les données dans apvts
            if(xmlState->hasTagName(apvts.state.getType()))
                apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
        else
        {
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

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            Params::Mix::id, Params::Mix::name, juce::NormalisableRange<float>(Params::Mix::min, Params::Mix::max, 0.01f),
            Params::Mix::init, juce::String(" %"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 1) + "%"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            Params::Gain::id, Params::Gain::name, juce::NormalisableRange<float>(Params::Gain::min, Params::Gain::max, 0.01f),
            Params::Gain::init, juce::String(" dB"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " dB"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            Params::Emission::id, Params::Emission::name,
            juce::NormalisableRange<float>(Params::Emission::min, Params::Emission::max, 0.001f), Params::Emission::init,
            juce::String(" g/s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " g/s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            Params::Duration::id, Params::Duration::name,
            juce::NormalisableRange<float>(Params::Duration::min, Params::Duration::max, 0.001f), Params::Duration::init,
            juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(Params::Speed::id, Params::Speed::name,
            juce::NormalisableRange<float>(Params::Speed::min, Params::Speed::max, 0.001f), Params::Speed::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(Params::Position::id, Params::Position::name,
            juce::NormalisableRange<float>(Params::Position::min, Params::Position::max, 0.001f), Params::Position::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(Params::Selection::id, Params::Selection::name,
            juce::NormalisableRange<float>(Params::Selection::min, Params::Selection::max, 0.001f), Params::Selection::init));

        layout.add(std::make_unique<juce::AudioParameterFloat>(Params::SustainRatio::id, Params::SustainRatio::name,
            juce::NormalisableRange<float>(Params::SustainRatio::min, Params::SustainRatio::max, 0.01f),
            Params::SustainRatio::init));

        // ENVELOPE MODE //
        const juce::StringArray choicesEnvModeNames(
            Params::EnvelopeMode::envModeNames.data(), (int)Params::EnvelopeMode::envModeNames.size());

        layout.add(std::make_unique<juce::AudioParameterChoice>(
            Params::EnvelopeMode::id, Params::EnvelopeMode::name, choicesEnvModeNames, 1));

        // TRAVERSAL MODE //

        const juce::StringArray choicesTraversalModeNames(
            Params::TraversalMode::traversalModeNames.data(), (int)Params::TraversalMode::traversalModeNames.size());

        layout.add(std::make_unique<juce::AudioParameterChoice>(
            Params::TraversalMode::id, Params::TraversalMode::name, choicesTraversalModeNames, 1));

        // TRAVERSAL FREQ //

        layout.add(std::make_unique<juce::AudioParameterFloat>(
            Params::TraversalFreq::id, Params::TraversalFreq::name,
            juce::NormalisableRange<float>(Params::TraversalFreq::min, Params::TraversalFreq::max, 0.01f),
            Params::TraversalFreq::init, juce::String(" Hz"), juce::AudioProcessorParameter::genericParameter,
            [](float v, int) { return juce::String(v, 2) + " Hz"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        // ajouter pan, direction
        // randomEMISSION, randomDuration, randomPan, randomDirection, randomPitch

        return layout;
    }

    void ParticulesAudioProcessor::setInputBuffer(AudioBuffer& buffer) noexcept
    {
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

        const AudioBuffer safeBuffer(tempBuffer);

        std::shared_ptr<const AudioBuffer> safeBufferPtr = std::make_shared<const AudioBuffer>(safeBuffer);

        granularEngine.setInputBuffer(std::move(safeBufferPtr));
    }

    void ParticulesAudioProcessor::initOnAudioLoadedCallback()
    {
        onAudioLoadedCallback = [this](AudioBuffer& buffer) {
            setInputBuffer(buffer);
            // TODO send UI Notification but do not change current file
            sendChangeMessage();
        };
    }

    void ParticulesAudioProcessor::loadFile(const juce::String& path) { loader.loadFile(path, onAudioLoadedCallback); }

    void ParticulesAudioProcessor::loadFile() { loader.loadFile(onAudioLoadedCallback); }

    void ParticulesAudioProcessor::loadDebugPreset()
    {
        //DBG("SAMPLE RATE = " + (juce::String) paramsView.getSampleRate());

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
        //DBG("LOADDEBUGPRESET");

        //juce::File debugFile = juce::File::getSpecialLocation(
        //	juce::File::userDesktopDirectory
        //).getChildFile("test.wav");

        juce::NormalisableRange<float> gainRange(Params::Gain::min, Params::Gain::max);
        const float normalizedGain = gainRange.convertTo0to1(Params::Gain::init);

        juce::NormalisableRange<float> EmissionRange(Params::Emission::min, Params::Emission::max);
        const float normalizedEmission = EmissionRange.convertTo0to1(10);

        juce::NormalisableRange<float> durationRange(Params::Duration::min, Params::Duration::max);
        const float normalizedDuration = durationRange.convertTo0to1(0.05f);

        juce::NormalisableRange<float> speedRange(Params::Speed::min, Params::Speed::max);
        const float normalizedSpeed = speedRange.convertTo0to1(1);

        juce::NormalisableRange<float> positionRange(Params::Position::min, Params::Position::max);
        const float normalizedPosition = positionRange.convertTo0to1(0);

        juce::NormalisableRange<float> selectionRange(Params::Selection::min, Params::Selection::max);
        const float normalizedSelection = selectionRange.convertTo0to1(0.25f);

        juce::NormalisableRange<float> TraversalFreqRange(Params::TraversalFreq::min, Params::TraversalFreq::max);
        const float normalizedTraversalFreq = TraversalFreqRange.convertTo0to1(1.f);

        juce::NormalisableRange<float> SustainRatioRange(Params::SustainRatio::min, Params::SustainRatio::max);
        const float normalizedSustainRatio = SustainRatioRange.convertTo0to1(0.5f);

        apvts.getParameter(Params::Mix::id)->setValueNotifyingHost(1.f); // MIX100%
        apvts.getParameter(Params::Gain::id)->setValueNotifyingHost(normalizedGain);
        apvts.getParameter(Params::Emission::id)->setValueNotifyingHost(normalizedEmission);
        apvts.getParameter(Params::Duration::id)->setValueNotifyingHost(normalizedDuration);
        apvts.getParameter(Params::Speed::id)->setValueNotifyingHost(normalizedSpeed);
        apvts.getParameter(Params::Position::id)->setValueNotifyingHost(normalizedPosition);
        apvts.getParameter(Params::Selection::id)->setValueNotifyingHost(normalizedSelection);
        apvts.getParameter(Params::TraversalFreq::id)->setValueNotifyingHost(normalizedTraversalFreq);
        apvts.getParameter(Params::SustainRatio::id)->setValueNotifyingHost(normalizedSustainRatio);
        apvts.getParameter(Params::EnvelopeMode::id)->setValueNotifyingHost(0.f);
        apvts.getParameter(Params::TraversalMode::id)->setValueNotifyingHost(0.f);

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
