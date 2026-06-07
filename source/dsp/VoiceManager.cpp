#include "VoiceManager.h"

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "utils/struct/AudioPayload.h"
#include "utils/struct/ParameterSnapshot.h"
#include "utils/struct/SmoothedParameters.h"

namespace particules
{
    VoiceManager::VoiceManager(SpawnGrainCallback c) : spawnCallback{c}, baseNote{60.f}, globalSampleCounter{0}
    {
        for(int i = 0; i < numPitchRatio; ++i)
            pitchRatioLUT[i] = std::pow(2.0f, (i - 60) / 12.0f);
    }

    void VoiceManager::setSampleRate(double sampleRate)
    {
        for(GranularVoice& voice : voices)
        {
            voice.setSampleRate(sampleRate);
            voice.setCallback(spawnCallback);
        }
    }

    void VoiceManager::setParameters(float a, float d, float s, float r, float e) noexcept
    {
        for(GranularVoice& voice : voices)
        {
            voice.setADSR(a, d, s, r);
            voice.setEmission(e);
        }
    }

    void VoiceManager::process(
        int currentSample, const ParameterSnapshot& ps, AudioPayload* payload /*, const SmoothedParameters& sp*/)
    {
#ifdef TRACY_ENABLE
        ZoneScopedN("VoiceManager::Process");
#endif
        globalSampleCounter++;
        for(int i = 0; i < params::maxMidiVoice; ++i)
        {
            GranularVoice& voice = voices[i];
            if(!voice.isBusy())
                continue;

            voice.tick(ps, payload, i);
        }
    }

    void VoiceManager::noteOn(int midiNoteNumber, float velocity)
    {
        GranularVoice* voice = findVoiceByNote(midiNoteNumber);

        // find next available voice
        if(voice == nullptr)
            voice = findFreeVoice();

        // voice stealing
        if(voice == nullptr)
            voice = findOldestVoice();

        // trigger the midi note
        if(voice != nullptr)
        {
            int safeNote = std::clamp(midiNoteNumber, 0, 127);
            float pitch = pitchRatioLUT[safeNote];
            voice->noteOn(midiNoteNumber, velocity, pitch, globalSampleCounter);
        }
    }

    void VoiceManager::noteOff(int midiNoteNumber)
    {
        // no breaks here
        // same note could exist on multiple voices
        // sustain pedal, stuck notes...
        for(GranularVoice& voice : voices)
            if(voice.isBusy() && voice.getCurrentNote() == midiNoteNumber)
                voice.noteOff();
    }

    void VoiceManager::allNotesOff()
    {
        for(GranularVoice& voice : voices)
            if(voice.isBusy())
                voice.noteOff();
    }

    bool VoiceManager::isAnyVoicesActive() const noexcept
    {
        for(const GranularVoice& voice : voices)
            if(voice.isBusy())
                return true;
        return false;
    }

    int VoiceManager::getNumActiveNotes() const noexcept
    {
        int count = 0;
        for(const GranularVoice& voice : voices)
            if(voice.isBusy())
                count++;
        return count;
    }

    bool VoiceManager::isVoiceDead(int index) const noexcept { return !voices[index].isBusy(); }

    float VoiceManager::getVoiceGain(int index) const noexcept { return 0.0f; }

    GranularVoice* VoiceManager::findOldestVoice()
    {
        GranularVoice* oldestVoice = nullptr;
        uint32_t time = UINT32_MAX;

        for(GranularVoice& voice : voices)
        {
            if(voice.isBusy() && voice.getNoteOnTime() < time)
            {
                oldestVoice = &voice;
                time = voice.getNoteOnTime();
            }
        }
        return oldestVoice;
    }

    GranularVoice* VoiceManager::findFreeVoice()
    {
        for(GranularVoice& voice : voices)
            if(!voice.isBusy())
                return &voice;
        return nullptr;
    }

    GranularVoice* VoiceManager::findVoiceByNote(int noteNumber)
    {
        for(GranularVoice& voice : voices)
            if(voice.isBusy() && voice.getCurrentNote() == noteNumber)
                return &voice;
        return nullptr;
    }
}