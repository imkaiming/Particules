#pragma once

//#include <juce_events/juce_events.h>
//#include <juce_audio_processors/juce_audio_processors.h>

#include "dsp/GranularEngine.h"
#include "framework/AudioFileLoader.h"
#include "framework/GrainVisualBuffer.h"
#include "framework/ParameterView.h"
#include "utils/CustomLookAndFeel.h"
#include "utils/UIContext.h"

class ParameterView;
class GranularEngine;
class ParticulesAudioProcessor : public juce::AudioProcessor,
                                 public juce::ChangeBroadcaster
#if JucePlugin_Enable_ARA
    ,
                                 public juce::AudioProcessorARAExtension
#endif
{
public:
    ParticulesAudioProcessor();
    ~ParticulesAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    ValueTreeState& getValueTreeState() noexcept { return apvts; };
    ParameterView& getParametersView() noexcept { return paramsView; };
    UIContext& getUIContext() noexcept { return uiContext; };

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void loadFile(const juce::String& path);
    void loadFile();
    const juce::File& getCurrentFile() const noexcept { return loader.getCurrentFile(); };
    AudioFileLoader& getAudioFileLoader() noexcept { return loader; };

    const int getNumActiveGrains() const noexcept { return granularEngine.getNumActiveGrains(); };

    void setInputBuffer(std::shared_ptr<const AudioBuffer> source) noexcept;
    const bool isInputBufferLoaded() const noexcept { return granularEngine.isInputBufferLoaded(); };

private:
    void initOnAudioLoadedCallback();
    void initSetInputBufferCallback();
    void loadDebugPreset();
    bool debugPresetLoaded = false;

    ValueTreeState apvts; // connecte les slider du GUI et les paramètres (fourni des valeurs atomiques)
    ParameterView paramsView; // fait le pont entre apvts et le synth
    GranularEngine granularEngine; // le moteur de la synthèse granulaire

    CustomLookAndFeel customLookAndFeel;
    UIContext uiContext;

    AudioFileLoader loader;
    std::function<void(const juce::File, bool)> onAudioLoadedCallback;
    std::function<void(std::shared_ptr<const AudioBuffer>)> setInputBufferCallback;

    GrainVisualBuffer visualBuffer;

    //juce::ADSR::Parameters adsrParameters;
    //juce::ADSR adsr;

    //juce::UndoManager undoManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessor)
};
