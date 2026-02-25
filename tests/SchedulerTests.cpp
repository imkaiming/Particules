#include "../source/dsp/Scheduler.h"
#include "../source/framework/ParameterSnapshot.h"
#include "../source/framework/SampleSource.h"
#include <catch2/catch_test_macros.hpp>

namespace audio_plugin_test
{
    struct SchedulerFixture
    {
        ParameterSnapshot snapshot;
        int numSamples;
        int numChannels;
        double sampleRate;

        SchedulerFixture()
        {
            snapshot.duration = 1.0f;
            snapshot.density = 1.f;
            snapshot.fadeInt = 0.f;
            snapshot.fadeOut = 0.f;
            snapshot.gain = 1.0f;
            snapshot.speed = 1.0f;
            snapshot.mix = 1.f;
            snapshot.envType = 1.f;
            snapshot.traversalMode = 1.f;
            snapshot.traversalTime = 1.f;

            numSamples = 512;
            numChannels = 2;
            sampleRate = 48000.0;
        }

        int spawnCallCount { 0 };
        std::vector<int> spawnPositions;

        void resetCounters()
        {
            spawnCallCount = 0;
            spawnPositions.clear();
        }

        void spawnCallback (int position, const ParameterSnapshot& p)
        {
            spawnCallCount++;
            spawnPositions.push_back (position);
        }
    };

    TEST_CASE ("1# Scheduler ctor", "[scheduler]")
    {
        Scheduler* s = nullptr;
        try
        {
            s = new Scheduler();
        } catch (...)
        {
            REQUIRE (false);
        }

        try
        {
            delete s;
        } catch (...)
        {
            REQUIRE (false);
        }

        REQUIRE (true);
    }

    TEST_CASE ("2# density = 1.f, sr = 512", "[scheduler]")
    {
        SchedulerFixture f;
        Scheduler scheduler;

        f.snapshot.density = 1.0f;
        f.numSamples = 512;
        f.sampleRate = 48000.0;
        try
        {
            scheduler.process (f.numSamples, f.sampleRate, f.snapshot.density, [&] (int pos, const ParameterSnapshot& p) { f.spawnCallback (pos, p); }, f.snapshot);
        } catch (const std::exception& e)
        {
            printf ("Exception: %s\n", e.what());
        }

        REQUIRE (f.spawnCallCount == 1);
    }

    TEST_CASE ("3# density = 0.f, sr = 512", "[scheduler]")
    {
        SchedulerFixture f;
        Scheduler scheduler;

        f.snapshot.density = 0.0f;
        f.numSamples = 512;
        f.sampleRate = 48000.0;

        try
        {
            scheduler.process (f.numSamples, f.sampleRate, f.snapshot.density, [&] (int pos, const ParameterSnapshot& p) { f.spawnCallback (pos, p); }, f.snapshot);
        } catch (const std::exception& e)
        {
            printf ("Exception: %s\n", e.what());
        }

        REQUIRE (f.spawnCallCount == 0);
    }

    TEST_CASE ("4# density = 500.f, sr = 64", "[scheduler]")
    {
        SchedulerFixture f;
        Scheduler scheduler;
        f.snapshot.density = 500.0f;
        f.numSamples = 64;
        f.sampleRate = 48000.0;

        try
        {
            // 500 / 48000.0 * 64 = 0.66666
            scheduler.process (f.numSamples, f.sampleRate, f.snapshot.density, [&] (int pos, const ParameterSnapshot& p) { f.spawnCallback (pos, p); }, f.snapshot);
        } catch (const std::exception& e)
        {
            printf ("Exception: %s\n", e.what());
        }

        REQUIRE (f.spawnCallCount == 1);
    }

    TEST_CASE ("5# density = 500.f, sr = 128", "[scheduler]")
    {
        SchedulerFixture f;
        Scheduler scheduler;
        f.snapshot.density = 500.0f;
        f.numSamples = 128;
        f.sampleRate = 48000.0;
        try
        {
            // 500 / 48000.0 * 64 = 0.66666
            scheduler.process (f.numSamples, f.sampleRate, f.snapshot.density, [&] (int pos, const ParameterSnapshot& p) { f.spawnCallback (pos, p); }, f.snapshot);
        } catch (const std::exception& e)
        {
            printf ("Exception: %s\n", e.what());
        }

        REQUIRE (f.spawnCallCount == 2);
    }
}
