#include "PluginProcessor.h"


#include "PluginEditor.h"

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
          core{*this}
#endif
    {
    }

    void ParticulesAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        core.prepareToPlay(sampleRate, samplesPerBlock);
    }

    void ParticulesAudioProcessor::releaseResources() {}

    void ParticulesAudioProcessor::processBlock(AudioBuffer& outputBuffer, juce::MidiBuffer& midiMessages)
    {
        core.processBlock(outputBuffer, midiMessages);
    }

    void ParticulesAudioProcessor::getStateInformation(juce::MemoryBlock& destData) { core.getStateInformation(destData); }

    void ParticulesAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
    {
        core.setStateInformation(data, sizeInBytes);
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
