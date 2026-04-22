#include "PluginCore.h"

#include <juce_audio_processors_headless/juce_audio_processors_headless.h> // audio parameter float

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "framework/core/PluginParams.h"
#include "utils/ParamHelpers.h"
#include "utils/struct/ParameterSnapshot.h"
#include "utils/struct/VisualSnapshot.h"

namespace particules
{
    PluginCore::PluginCore(juce::AudioProcessor& p)
        : proc{p}, apvts(p, nullptr, "Parameters", createParameterLayout()), paramState{}, granularEngine{faudio}, audioState{},
          uiState{}, uic{apvts, paramState, audioState, uiState, fui, faudio}, loader{}, debugPresetLoaded{false},
          incomingBuffer{}, garbageCollector{}, currentPayload{nullptr}, wasAuditioning{false},
          synchronizer{currentPayload, garbageCollector, audioState, uiState}, faudio{audioState, visualBuffer}
    {
        onAudioLoadedCallback = [this](std::unique_ptr<AudioBuffer> buffer, const juce::File& loadedFile) {
            AudioPayload* newPayload = new AudioPayload();

            newPayload->buffer = std::move(buffer);
            newPayload->numSamples = newPayload->buffer->getNumSamples() - 1; // minus the added guard sample
            newPayload->numChannels = newPayload->buffer->getNumChannels();
            newPayload->file = loadedFile;

            incomingBuffer.push(newPayload);
        };

        fui.onLoadFile = [this] { loader.loadFile(onAudioLoadedCallback, uiState.getCurrentFile()); };
        fui.onLoadFilePath = [this](const str& path) { loader.loadFile(path, onAudioLoadedCallback, uiState.getCurrentFile()); };
        fui.onSetPlaying = [this](bool play) {
            if(juce::RangedAudioParameter* param = apvts.getParameter(params::play::id))
            {
                param->beginChangeGesture();
                param->setValueNotifyingHost(param->convertTo0to1(play ? 1.0f : 0.0f));
                param->endChangeGesture();
            }
        };
        fui.onIsPlaying = [this]() -> float { return paramState.getPlay() > 0.5f ? 1.0f : 0.f; };

        synchronizer.start(10);
    }

    PluginCore::~PluginCore()
    {
        synchronizer.stop();

        while(AudioPayload* payload = incomingBuffer.pop())
            delete payload;

        while(AudioPayload* payload = garbageCollector.pop())
            delete payload;

        AudioPayload* active = currentPayload.exchange(nullptr);
        if(active)
            delete active;
    }

    void PluginCore::prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        const int numChannels = proc.getTotalNumOutputChannels();
        paramState.init(apvts);
        audioState.setSampleRate(sampleRate);
        uiState.init(&visualBuffer);
        granularEngine.init(sampleRate, numChannels, samplesPerBlock);
        loader.init(sampleRate, numChannels);
    }

    void PluginCore::processBlock(AudioBuffer& outputBuffer, juce::MidiBuffer& midiBuffer)
    {
#ifdef TRACY_ENABLE 
        ZoneScoped; // tracy submodules
#endif

        // security for pluginval
        outputBuffer.clear();

        AudioPayload* payload = currentPayload.load(std::memory_order_relaxed);
        bool fileSwappedThisBlock = false;

        // 1. new payload verification
        while(AudioPayload* next = incomingBuffer.pop())
        {
            if(payload)
                garbageCollector.push(payload);

            payload = next;
        }

        // 2. set the new payload as the current
        currentPayload.store(payload, std::memory_order_release);

        if(payload == nullptr)
            return;

        ParameterSnapshot ps = paramState.getSnapshot(payload, audioState.getSampleRate());

        if(!ps.isValid())
            return;

        const int inputuNumChannels = proc.getTotalNumInputChannels();
        const int outputNumChannels = proc.getTotalNumOutputChannels();
        const int bufferSize = outputBuffer.getNumSamples();
        float* const* outputPtrs = outputBuffer.getArrayOfWritePointers();

        //bool currentlyAuditioning = audioState.getIsAuditioning();

        keyboardState.processNextMidiBuffer(midiBuffer, 0, bufferSize, true);

        granularEngine.process(outputBuffer, midiBuffer, payload, bufferSize, outputPtrs, outputNumChannels, ps);
    }

    void PluginCore::getStateInformation(juce::MemoryBlock& destData)
    {
        juce::ValueTree vt = apvts.copyState();

        juce::ValueTree playParam = vt.getChildWithName(params::play::id);
        if(playParam.isValid())
            playParam.setProperty("value", 0.0f, nullptr);

        juce::ValueTree customState("particulesCustomState");
        const juce::File& f = uiState.getCurrentFile();
        if(f.existsAsFile())
        {
            // adding the input buffer to the value tree
            juce::ValueTree audioFileNode("AudioFile");
            customState.setProperty("audioFilePath", f.getFullPathName(), nullptr);
        }

        //runtimeParametersNode.setProperty("isPlaying", engineState.getIsPlaying(), nullptr);
        customState.setProperty("numSamples", audioState.getNumSamples(), nullptr);
        customState.setProperty("sampleRate", audioState.getSampleRate(), nullptr);
        customState.setProperty("numChannels", audioState.getNumChannels(), nullptr);
        vt.addChild(customState, -1, nullptr);

        //serialization to binary
        std::unique_ptr<juce::XmlElement> xml(vt.createXml());

        proc.copyXmlToBinary(*xml, destData);
    }

    void PluginCore::setStateInformation(const void* data, int sizeInBytes)
    {
        std::unique_ptr<juce::XmlElement> xmlState(proc.getXmlFromBinary(data, sizeInBytes));
        if(xmlState == nullptr)
            return;

        // si on trouve un fichier xml alors on met à jours les données dans apvts
        if(xmlState->hasTagName(apvts.state.getType()))
        {
            juce::ValueTree vt = juce::ValueTree::fromXml(*xmlState);

            juce::ValueTree customState = vt.getChildWithName("particulesCustomState");
            str filePathToLoad;
            if(customState.isValid())
            {
                //engineState.setIsPlaying(runtimeParametersNode.getProperty("isPlaying"));
                audioState.setNumChannels(customState.getProperty("numChannels"));
                audioState.setSampleRate(customState.getProperty("sampleRate"));
                audioState.setNumSamples(customState.getProperty("numSamples"));
                loader.setSampleRate(audioState.getSampleRate());
                loader.setNumTargetChannels(audioState.getNumChannels());

                filePathToLoad = customState.getProperty("audioFilePath", "").toString();
                vt.removeChild(customState, nullptr);
            }
            apvts.replaceState(vt);
            if(filePathToLoad.isNotEmpty())
            {
                juce::File f{filePathToLoad};
                if(f.existsAsFile())
                    fui.loadFilePath(filePathToLoad);
            }
        }
    }

    ValueTreeState::ParameterLayout PluginCore::createParameterLayout()
    {
        juce::AudioProcessorValueTreeState::ParameterLayout layout;

        // PLAY //
        layout.add(createNormalizedParameter(params::play::id, params::play::name, params::play::min, params::play::max,
            params::play::skewFactor, params::play::init));

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

        // PAN //
        layout.add(createNormalizedParameter(
            params::pan::id, params::pan::name, params::pan::min, params::pan::max, params::pan::skewFactor, params::pan::init,
            [](float v, int) -> str { return ""; }, [](const str& s) -> float { return s.getFloatValue(); }));

        layout.add(createNormalizedParameter(params::pan_jitter::id, params::pan_jitter::name, params::pan_jitter::min,
            params::pan_jitter::max, params::pan_jitter::skewFactor, params::pan_jitter::init));

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

    void PluginCore::loadDebugPreset()
    {
        juce::File projectRoot(PROJECT_SOURCE_DIR);
        juce::File debugAudio = projectRoot.getChildFile("resources").getChildFile("audio").getChildFile("01_Piano_E.wav");

        if(debugPresetLoaded)
            return;

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
        apvts.getParameter(params::playback::id)->setValueNotifyingHost(1.f);
        apvts.getParameter(params::play::id)->setValueNotifyingHost(0.f);

        if(debugAudio.existsAsFile())
            fui.loadFilePath(debugAudio.getFullPathName());

        debugPresetLoaded = true;
    }
}