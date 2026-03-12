#include "../source/dsp/Scheduler.h"
#include "../source/utils/ParameterSnapshot.h"
#include "../source/utils/TraversalMode.h"
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
            snapshot.envMode = static_cast<EnvelopeMode>(1);
            snapshot.traversalMode = TraversalMode::Sine;
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

    TEST_CASE_METHOD(SchedulerFixture, "Scheduler emission spawn correct grain", "[scheduler]")
    {
        SECTION("1# emission = 1.f, sr = 512")
        {
            Scheduler scheduler;

            snapshot.emission = 1.0f;
            numSamples = 512;
            sampleRate = 48000.0;

            scheduler.process(
                numSamples, sampleRate, snapshot.emission, [&](int pos, const ParameterSnapshot& p) { spawnCallback(pos, p); },
                snapshot);

            REQUIRE(spawnCallCount == 1);
        }

        SECTION("3# emission = 0.f, sr = 512")
        {
            Scheduler scheduler;

            snapshot.emission = 0.0f;
            numSamples = 512;
            sampleRate = 48000.0;

            scheduler.process(
                numSamples, sampleRate, snapshot.emission, [&](int pos, const ParameterSnapshot& p) { spawnCallback(pos, p); },
                snapshot);

            REQUIRE(spawnCallCount == 0);
        }

        SECTION("4# emission = 500.f, sr = 64")
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

        SECTION("5# emission = 500.f, sr = 128")
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

        SECTION("6# emission = 500.f, sr = 64, 3 iterations")
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
}
