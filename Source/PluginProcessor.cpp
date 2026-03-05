/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "framework/ParamsID.h"

//==============================================================================
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
      paramsView(), grainEngine(paramsView), uiContext{apvts, paramsView, customLookAndFeel, *this}, loader{paramsView},
      debugPresetLoaded{false}
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
    int numChannels = getTotalNumOutputChannels();
    paramsView.init(apvts, sampleRate);
    grainEngine.init(static_cast<int>(sampleRate), numChannels, samplesPerBlock);

    //juce::Logger::outputDebugString("EMISSION : " + (const juce::String)this->paramsView.getEMISSION() + " duration : " + (const juce::String)this->paramsView.getDuration());
#if ENABLE_DEBUG_PRESET // exist also in the audio file loader
    loadDebugPreset();
#endif
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
    const int totalNumInputChannels = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    for(int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, numSamples);

    if(paramsView.getAudioSource() && paramsView.getIsPlaying() || !paramsView.getIsGrainsEmpty())
    {
        grainEngine.process(buffer, numSamples);
    }
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
        Param::Mix::id, Param::Mix::name, juce::NormalisableRange<float>(Param::Mix::min, Param::Mix::max, 0.01f),
        Param::Mix::init, juce::String(" %"), juce::AudioProcessorParameter::genericParameter,
        [](float v, int) { return juce::String(v, 1) + "%"; }, [](const juce::String& s) { return s.getFloatValue(); }));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Param::Gain::id, Param::Gain::name, juce::NormalisableRange<float>(Param::Gain::min, Param::Gain::max, 0.01f),
        Param::Gain::init, juce::String(" dB"), juce::AudioProcessorParameter::genericParameter,
        [](float v, int) { return juce::String(v, 2) + " dB"; }, [](const juce::String& s) { return s.getFloatValue(); }));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Param::Emission::id, Param::Emission::name,
        juce::NormalisableRange<float>(Param::Emission::min, Param::Emission::max, 0.001f), Param::Emission::init,
        juce::String(" g/s"), juce::AudioProcessorParameter::genericParameter,
        [](float v, int) { return juce::String(v, 2) + " g/s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

        layout.add(std::make_unique<juce::AudioParameterFloat>(
        Param::Duration::id, Param::Duration::name,
        juce::NormalisableRange<float>(Param::Duration::min, Param::Duration::max, 0.001f), Param::Duration::init,
        juce::String(" s"), juce::AudioProcessorParameter::genericParameter,
        [](float v, int) { return juce::String(v, 2) + " s"; }, [](const juce::String& s) { return s.getFloatValue(); }));

    layout.add(std::make_unique<juce::AudioParameterFloat>(Param::Speed::id, Param::Speed::name,
        juce::NormalisableRange<float>(Param::Speed::min, Param::Speed::max, 0.001f), Param::Speed::init));

    //layout.add(
    //	std::make_unique<juce::AudioParameterFloat>(
    //		PITCH_ID,
    //		PITCH_NAME,
    //		PITCH_MIN, PITCH_MAX, PITCH_DEFAULT)
    //);

    //layout.add(
    //	std::make_unique<juce::String>(
    //		FILEPATH_ID,
    //		FILEPATH_NAME,
    //		FILEPATH_DEFAULT)
    //);

    layout.add(std::make_unique<juce::AudioParameterFloat>(Param::Position::id, Param::Position::name,
        juce::NormalisableRange<float>(Param::Position::min, Param::Position::max, 0.001f), Param::Position::init));

    layout.add(std::make_unique<juce::AudioParameterFloat>(Param::Selection::id, Param::Selection::name,
        juce::NormalisableRange<float>(Param::Selection::min, Param::Selection::max, 0.001f), Param::Selection::init));

    const juce::StringArray choicesEnvTypeNames(
        Param::EnvelopeType::envTypeNames.data(), (int)Param::EnvelopeType::envTypeNames.size());
    //const juce::StringArray choices{Param::EnvelopeType::envTypeNames, sizeof(Param::EnvelopeType::envTypeNames)};

    layout.add(
        std::make_unique<juce::AudioParameterChoice>(Param::EnvelopeType::id, Param::EnvelopeType::name, choicesEnvTypeNames, 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>(Param::SustainRatio::id, Param::SustainRatio::name,
        juce::NormalisableRange<float>(Param::SustainRatio::min, Param::SustainRatio::max, 0.01f), Param::SustainRatio::init));

    //const juce::StringArray choicesTraversalModeNames(Param::TraversalMode::traversalModeNames.data(), Param::TraversalMode::traversalModeNames.size());

    layout.add(std::make_unique<juce::AudioParameterInt>(
        Param::TraversalMode::id, Param::TraversalMode::name, 1, 5, Param::TraversalMode::init));

    //layout.add(std::make_unique<juce::AudioParameterFloat>(Param::TraversalFreq::id, Param::TraversalFreq::name,
    //    juce::NormalisableRange<float>(Param::TraversalFreq::min, Param::TraversalFreq::max, 0.01f), Param::TraversalFreq::init));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        Param::TraversalFreq::id, Param::TraversalFreq::name,
        juce::NormalisableRange<float>(Param::TraversalFreq::min, Param::TraversalFreq::max, 0.01f), Param::TraversalFreq::init,
        juce::String(" Hz"), juce::AudioProcessorParameter::genericParameter,
        [](float v, int) { return juce::String(v, 2) + " Hz"; }, [](const juce::String& s) { return s.getFloatValue(); }));

    // ajouter pan, direction
    // randomEMISSION, randomDuration, randomPan, randomDirection, randomPitch

    return layout;
}

void ParticulesAudioProcessor::initOnAudioLoadedCallback()
{
    onAudioLoadedCallback = [this](juce::File f, bool ok) {
        if(ok)
        {
            currentFile = f;
        }
        else
        {
            // TODO send UI Notification but do not change current file
        }
        sendChangeMessage();
    };
}

void ParticulesAudioProcessor::loadFile(const juce::String& path) { loader.loadFile(path, onAudioLoadedCallback); }

void ParticulesAudioProcessor::loadFile() { loader.loadFile(onAudioLoadedCallback); }

void ParticulesAudioProcessor::loadDebugPreset()
{
    //DBG("SAMPLE RATE = " + (juce::String) paramsView.getSampleRate());

    juce::File debugAudio = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
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

    juce::NormalisableRange<float> gainRange(Param::Gain::min, Param::Gain::max);
    float normalizedGain = gainRange.convertTo0to1(Param::Gain::init);

    juce::NormalisableRange<float> EmissionRange(Param::Emission::min, Param::Emission::max);
    float normalizedEmission = EmissionRange.convertTo0to1(1);

    juce::NormalisableRange<float> durationRange(Param::Duration::min, Param::Duration::max);
    float normalizedDuration = durationRange.convertTo0to1(1);

    juce::NormalisableRange<float> speedRange(Param::Speed::min, Param::Speed::max);
    float normalizedSpeed = speedRange.convertTo0to1(1);

    juce::NormalisableRange<float> positionRange(Param::Position::min, Param::Position::max);
    float normalizedPosition = positionRange.convertTo0to1(0.5);

    juce::NormalisableRange<float> selectionRange(Param::Selection::min, Param::Selection::max);
    float normalizedSelection = selectionRange.convertTo0to1(0.25f);

    juce::NormalisableRange<float> TraversalFreqRange(Param::TraversalFreq::min, Param::TraversalFreq::max);
    float normalizedTraversalFreq = TraversalFreqRange.convertTo0to1(10.f);

    juce::NormalisableRange<float> SustainRatioRange(Param::SustainRatio::min, Param::SustainRatio::max);
    float normalizedSustainRatio = SustainRatioRange.convertTo0to1(0.5f);

    apvts.getParameter(Param::Mix::id)->setValueNotifyingHost(1.f); // MIX100%
    apvts.getParameter(Param::Gain::id)->setValueNotifyingHost(normalizedGain);
    apvts.getParameter(Param::Emission::id)->setValueNotifyingHost(normalizedEmission);
    apvts.getParameter(Param::Duration::id)->setValueNotifyingHost(normalizedDuration);
    apvts.getParameter(Param::Speed::id)->setValueNotifyingHost(normalizedSpeed);
    apvts.getParameter(Param::Position::id)->setValueNotifyingHost(normalizedPosition);
    apvts.getParameter(Param::Selection::id)->setValueNotifyingHost(normalizedSelection);
    apvts.getParameter(Param::EnvelopeType::id)->setValueNotifyingHost(0.f);
    apvts.getParameter(Param::TraversalFreq::id)->setValueNotifyingHost(normalizedTraversalFreq);
    apvts.getParameter(Param::SustainRatio::id)->setValueNotifyingHost(normalizedSustainRatio);

    if(debugAudio.existsAsFile())
    {
        this->loadFile(debugAudio.getFullPathName());
    }

    //if(paramsView.getSampleSource())
    //	DBG("SAMPLESOURCE OK");

    //apvts.getParameter(Param::EnvelopeWidth::id)->setValueNotifyingHost(100.f);
    //apvts.getParameter(Param::TraversalMode::id)->;
    //apvts.getParameter(Param::TraversalFreq::id)->;

    debugPresetLoaded = true;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new ParticulesAudioProcessor(); }
