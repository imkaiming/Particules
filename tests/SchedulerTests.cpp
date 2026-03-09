#include "../source/dsp/Scheduler.h"
#include "../source/framework/ParameterSnapshot.h"
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
            snapshot.durationSamples = 48000;
            snapshot.emission = 1.f;
            snapshot.linearGain = 1.0f;
            snapshot.speed = 1.0f;
            snapshot.mix = 1.f;
            snapshot.envType = 1.f;
            snapshot.traversalMode = 1;
            snapshot.traversalFreq = 1.f;

            numSamples = 512;
            numChannels = 2;
            sampleRate = 48000.0;
        }

        int spawnCallCount{0};
        std::vector<int> spawnPositions;

        void resetCounters()
        {
            spawnCallCount = 0;
            spawnPositions.clear();
        }

        void spawnCallback(int position, const ParameterSnapshot& p)
        {
            spawnCallCount++;
            spawnPositions.push_back(position);
        }
    };

    TEST_CASE("1# Scheduler ctor", "[scheduler]") { Scheduler s; }

    TEST_CASE("2# Emission = 1.f, sr = 512", "[scheduler]")
    {
        SchedulerFixture f;
        Scheduler scheduler;

        f.snapshot.emission = 1.0f;
        f.numSamples = 512;
        f.sampleRate = 48000.0;

        scheduler.process(
            f.numSamples, f.sampleRate, f.snapshot.emission,
            [&](int pos, const ParameterSnapshot& p) { f.spawnCallback(pos, p); }, f.snapshot);

        REQUIRE(f.spawnCallCount == 1);
    }

    TEST_CASE("3# Emission = 0.f, sr = 512", "[scheduler]")
    {
        SchedulerFixture f;
        Scheduler scheduler;

        f.snapshot.emission = 0.0f;
        f.numSamples = 512;
        f.sampleRate = 48000.0;

        REQUIRE_THROWS(scheduler.process(
            f.numSamples, f.sampleRate, f.snapshot.emission,
            [&](int pos, const ParameterSnapshot& p) { f.spawnCallback(pos, p); }, f.snapshot));

        REQUIRE(f.spawnCallCount == 0);
    }

    TEST_CASE_METHOD(SchedulerFixture, "4# Emission = 500.f, sr = 64", "[scheduler]")
    {
        Scheduler scheduler;
        snapshot.emission = 500.0f;
        numSamples = 64;
        sampleRate = 48000.0;

        // 500 / 48000.0 * 64 = 0.66666
        scheduler.process(
            numSamples, sampleRate, snapshot.emission, [&](int pos, const ParameterSnapshot& p) { spawnCallback(pos, p); },
            snapshot);

        REQUIRE(spawnCallCount == 1);
    }

    TEST_CASE_METHOD(SchedulerFixture, "5# Emission = 500.f, sr = 128", "[scheduler]")
    {
        Scheduler scheduler;
        snapshot.emission = 500.0f;
        numSamples = 128;
        sampleRate = 48000.0;
        // 500 / 48000.0 * 64 = 0.66666
        scheduler.process(
            numSamples, sampleRate, snapshot.emission, [&](int pos, const ParameterSnapshot& p) { spawnCallback(pos, p); },
            snapshot);

        REQUIRE(spawnCallCount == 2);
    }

    TEST_CASE_METHOD(SchedulerFixture, "6# Emission = 500.f, sr = 64, 3 iterations")
    {
        Scheduler scheduler;
        snapshot.emission = 500.0f;
        numSamples = 64;
        sampleRate = 48000.0;
        // 48000 / 500 = 96
        scheduler.process(
            numSamples, sampleRate, snapshot.emission, [&](int pos, const ParameterSnapshot& p) { spawnCallback(pos, p); },
            snapshot);

        REQUIRE(spawnCallCount == 1);
        REQUIRE(spawnPositions.at(0) == 0);

        scheduler.process(
            numSamples, sampleRate, snapshot.emission, [&](int pos, const ParameterSnapshot& p) { spawnCallback(pos, p); },
            snapshot);

        REQUIRE(spawnCallCount == 2);
        REQUIRE(spawnPositions.at(1) == 32);

        scheduler.process(
            numSamples, sampleRate, snapshot.emission, [&](int pos, const ParameterSnapshot& p) { spawnCallback(pos, p); },
            snapshot);

        REQUIRE(spawnCallCount == 2);
        REQUIRE(spawnPositions.size() == 2);

        scheduler.process(
            numSamples, sampleRate, snapshot.emission, [&](int pos, const ParameterSnapshot& p) { spawnCallback(pos, p); },
            snapshot);

        REQUIRE(spawnCallCount == 3);
        REQUIRE(spawnPositions.at(2) == 0);
        REQUIRE(spawnPositions.size() == 3);
    }
}
