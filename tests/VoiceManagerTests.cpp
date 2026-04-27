#include <catch2/catch_test_macros.hpp>
#include <map>

#include "dsp/Grain.h"
#include "dsp/GranularVoice.h"
#include "dsp/VoiceManager.h"
#include "framework/core/PluginParams.h"
#include "utils/struct/AudioPayload.h"
#include "utils/struct/ParameterSnapshot.h"

namespace particulesTest
{
    using namespace particules;
    inline constexpr int maxVoice = params::maxMidiVoice;

    struct VoiceManagerFixture
    {
        ParameterSnapshot ps;
        AudioPayload payload;
        double sampleRate;
        std::map<int, float> voiceToPitchMap;
        int sampleBlock = 512;
        VoiceManager vm;

        VoiceManagerFixture()
            : vm{[&](const ParameterSnapshot&, AudioPayload*, int voiceID, float pitchRatio, float) {
                  voiceToPitchMap[voiceID] = pitchRatio;
              }}
        {
            sampleRate = 48000.0;
            ps.attack = 0.0f;
            ps.decay = 0.0f;
            ps.sustain = 1.0f;
            ps.release = 0.0f;
            ps.emission = 100.0f;

            vm.setSampleRate(sampleRate);
            vm.setParameters(ps.attack, ps.decay, ps.sustain, ps.release, ps.emission);
        }
    };

    TEST_CASE_METHOD(VoiceManagerFixture, "VoiceManager allocates a free voice and assigns correct pitch", "[voice_manager]")
    {
        vm.noteOn(60, 1.0f);

        for(int s = 0; s < 4800; ++s)
            vm.process(s, ps, &payload);

        // with base note pitch ratio should be 1.0f
        REQUIRE(vm.getNumActiveNotes() == 1);
        REQUIRE(voiceToPitchMap.size() == 1);

        // assuming the first available voice is index 0
        REQUIRE(voiceToPitchMap.contains(0));
        REQUIRE(voiceToPitchMap[0] == 1.0f);

        vm.noteOn(72, 1.0f);
        for(int s = 0; s < 4800; ++s)
            vm.process(s, ps, &payload);

        // note 72 is 1 octaveup so pitch ratio should be 2.0f
        REQUIRE(vm.getNumActiveNotes() == 2);
        REQUIRE(voiceToPitchMap.contains(1));
        REQUIRE(voiceToPitchMap[1] == 2.0f);

        vm.noteOn(48, 1.0f);
        for(int s = 0; s < 4800; ++s)
            vm.process(s, ps, &payload);

        // note 48 is 1 octave down so pitch ratio should be 0.5f
        REQUIRE(vm.getNumActiveNotes() == 3);
        REQUIRE(voiceToPitchMap.contains(2));
        REQUIRE(voiceToPitchMap[2] == 0.5f);
    }

    TEST_CASE_METHOD(VoiceManagerFixture, "VoiceManager frees a voice after Note Off and release phase", "[voice_manager]")
    {
        vm.noteOn(60, 1.0f);
        vm.process(0, ps, &payload);
        REQUIRE(vm.getNumActiveNotes() == 1);

        vm.noteOff(60);

        //release is 0.0f so processing should instantly exhaust the voice.
        vm.process(1, ps, &payload);

        REQUIRE(vm.getNumActiveNotes() == 0);
    }

    TEST_CASE_METHOD(
        VoiceManagerFixture, "VoiceManager retriggers the same voice if the same note is played twice", "[voice_manager]")
    {
        vm.noteOn(60, 1.0f);
        for(int s = 0; s < 4800; ++s)
            vm.process(0, ps, &payload);
        REQUIRE(vm.getNumActiveNotes() == 1);

        // retrigger the exact same note again without calling note Off
        vm.noteOn(60, 1.0f);
        for(int s = 0; s < 4800; ++s)
            vm.process(1, ps, &payload);

        // The manager should reuse the existing voice mapped to note 60
        REQUIRE(vm.getNumActiveNotes() == 1);
    }

    TEST_CASE_METHOD(VoiceManagerFixture, "VoiceManager instantly kills all voices on allNotesOff", "[voice_manager]")
    {
        vm.noteOn(60, 1.0f);
        vm.noteOn(64, 1.0f);
        vm.noteOn(67, 1.0f);

        for(int s = 0; s < 4800; ++s)
            vm.process(0, ps, &payload);

        REQUIRE(vm.getNumActiveNotes() == 3);

        vm.allNotesOff();
        for(int s = 0; s < 4800; ++s)
            vm.process(1, ps, &payload);

        REQUIRE(vm.getNumActiveNotes() == 0);
    }

    TEST_CASE_METHOD(VoiceManagerFixture, "VoiceManager steals the oldest voice when maxed out", "[voice_manager]")
    {
        // incrementing one sample per note
        for(int i = 0; i < maxVoice; ++i)
        {
            vm.noteOn(60 + i, 1.0f);
            for(int s = 0; s < 4800; ++s)
                vm.process(i, ps, &payload);
        }

        // the engine should be full
        REQUIRE(vm.getNumActiveNotes() == maxVoice);
        REQUIRE(voiceToPitchMap.size() == maxVoice);

        // voice 0 should be base note 60
        const float oldestPitchRatio = voiceToPitchMap[0];

        // voice stealing (triggering the 9th note)
        vm.noteOn(75, 1.0f);
        for(int s = 0; s < 4800; ++s)
            vm.process(maxVoice, ps, &payload);

        REQUIRE(vm.getNumActiveNotes() == maxVoice);

        // voice 0's pitch ratio must have changed
        REQUIRE(voiceToPitchMap[0] != oldestPitchRatio);

        // voice 1 should'nt have changed
        float secondOldestExpectedPitch = voiceToPitchMap[1];
        REQUIRE(voiceToPitchMap[1] == secondOldestExpectedPitch);
    }
}