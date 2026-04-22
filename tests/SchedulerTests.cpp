#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "dsp/Scheduler.h"
#include "utils/struct/ParameterSnapshot.h"
#include "utils/struct/AudioPayload.h"

using namespace particules;

namespace audio_plugin_test
{
    struct SchedulerFixture
    {
        ParameterSnapshot snapshot;
        AudioPayload payload;
        double sampleRate = 48000.0;
        int index = -1;
        float pitch = 1.0f;
        float gain = 1.0f;


        SchedulerFixture()
        {
            snapshot.durationSamples = 48000;
            snapshot.emission = 1.f;
            // Ensure jitter is 0 by default for deterministic tests
            //snapshot.emissionJitter = 0.0f;
        }

        int spawnCount = 0;

        auto makeSpawnCallback()
        {
            return [this](const ParameterSnapshot&, AudioPayload*, int, float, float) { spawnCount++; };
        }
    };

    // 1. BOUNDARY TESTS
    TEST_CASE_METHOD(SchedulerFixture, "emission below minimum must clamps to params::emission::min", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setSampleRate(sampleRate);
        scheduler.setEmission(0.f); 

        for(int i = 0; i < static_cast<int>(sampleRate); ++i)
        {
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);
        }

        REQUIRE(spawnCount == 0);
    }

    TEST_CASE_METHOD(SchedulerFixture, "emission at maximum limits to params::emission::max", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setEmission(500.f);
        scheduler.setSampleRate(sampleRate);

        for(int i = 0; i < static_cast<int>(sampleRate); ++i)
        {
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);
        }

        REQUIRE(spawnCount == static_cast<int>(params::emission::max)); // 50
    }

    TEST_CASE_METHOD(SchedulerFixture, "emission above maximum must clamps to params::emission::max", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setEmission(1000.f);
        scheduler.setSampleRate(sampleRate);

        for(int i = 0; i < static_cast<int>(sampleRate); ++i)
        {
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);
        }

        REQUIRE(spawnCount == static_cast<int>(params::emission::max));
    }

    // 2. BASIC FREQUENCY TESTS
    TEST_CASE_METHOD(SchedulerFixture, "emission = 1 grain per second", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setEmission(1.0f);
        scheduler.setSampleRate(sampleRate);

        for(int i = 0; i < static_cast<int>(sampleRate); ++i)
        {
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);
        }

        REQUIRE(spawnCount == 1);
    }

    TEST_CASE_METHOD(SchedulerFixture, "emission = 50 grains per second", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setEmission(50.0f);
        scheduler.setSampleRate(sampleRate);

        for(int i = 0; i < static_cast<int>(sampleRate); ++i)
        {
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);
        }

        REQUIRE(spawnCount == 50);
    }

    // 3. ACCURACY
    TEST_CASE_METHOD(SchedulerFixture, "phase accumulation should stay exact even with irrational emission", "[scheduler]")
    {
        Scheduler scheduler;
        // 7 Hz does not divide perfectly into 48000
        scheduler.setEmission(7.0f);
        scheduler.setSampleRate(sampleRate);

        for(int i = 0; i < static_cast<int>(sampleRate) * 10; ++i)
        {
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);
        }

        REQUIRE(spawnCount == 70);
    }

    TEST_CASE_METHOD(SchedulerFixture, "reset() clears phase accumulation", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setSampleRate(sampleRate);
        scheduler.setEmission(2.0f);

        for(int i = 0; i < 12000; ++i)
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);

        REQUIRE(spawnCount == 0);

        scheduler.reset();

        spawnCount = 0;
        for(int i = 0; i < 23999; ++i)
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);

        REQUIRE(spawnCount == 1);

        scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);
        REQUIRE(spawnCount == 1);
    }

    TEST_CASE_METHOD(SchedulerFixture, "no drift over long durations", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setEmission(10.0f);
        scheduler.setSampleRate(sampleRate);

        // Run for 10 seconds
        for(int i = 0; i < static_cast<int>(sampleRate) * 10; ++i)
        {
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);
        }

        REQUIRE(spawnCount == 100);
    }

    // 4. CHANGING DYNAMICALLY THE PARAMETERS
    TEST_CASE_METHOD(SchedulerFixture, "setEmission updates interval mid-stream", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setEmission(1.0f);
        scheduler.setSampleRate(sampleRate);

        for(int i = 0; i < 24000; ++i)
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);

        REQUIRE(spawnCount == 1);

        scheduler.setEmission(2.0f);
        spawnCount = 0;

        for(int i = 0; i < 24000; ++i)
            scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain);

        // Au bout de la seconde moitié, le 2ème grain est parti.
        REQUIRE(spawnCount == 1);
    }
    
    // 5. BAD STATE AND C++ SAFETY
    TEST_CASE_METHOD(SchedulerFixture, "tick before setSampleRate does not explode", "[scheduler]")
    {
        Scheduler scheduler;
        // omission of scheduler.setSampleRate(sampleRate);
        // Internal sampleRate is 0.0

        scheduler.setEmission(10.0f);

        for(int i = 0; i < 100; ++i)
        {
            REQUIRE_NOTHROW(scheduler.tick(makeSpawnCallback(), snapshot, &payload, index, pitch, gain));
        }

        // Safety check: it shouldn't randomly spawn a million grains either
        REQUIRE(spawnCount == 0);
    }

    TEST_CASE_METHOD(SchedulerFixture, "callback receives correct snapshot reference", "[scheduler]")
    {
        Scheduler scheduler;
        scheduler.setEmission(params::emission::max); 
        scheduler.setSampleRate(sampleRate);

        ParameterSnapshot captured;
        bool wasCalled = false;

        auto capture = [&](const ParameterSnapshot& ps, AudioPayload* payload, int index, float pitch, float gain) {
            captured = ps;
            wasCalled = true;
        };

        scheduler.tick(capture, snapshot, &payload, index, pitch, gain);

        REQUIRE(wasCalled);
        REQUIRE(captured.durationSamples == snapshot.durationSamples);
        REQUIRE(captured.emission == snapshot.emission);
        REQUIRE(captured.linearGain == snapshot.linearGain);
    }
}