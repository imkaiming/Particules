#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "dsp/Grain.h"
#include "utils/struct/ParameterSnapshot.h"

using namespace particules;

namespace audio_plugin_test
{
    struct GrainFixture
    {
        Grain grain;
        ParameterSnapshot snapshot;

        GrainFixture()
        {
            snapshot.inputNumSamples = 1000;
            snapshot.durationSamples = 100;
            snapshot.startPositionSamples = 0;
            snapshot.spanSamples = 0;
            snapshot.speed = 1.0f;
            snapshot.sustainRatio = 0.5f;
            snapshot.playback = 1.f;
        }
    };

    // 1. MODULATION POSITION
    TEST_CASE_METHOD(GrainFixture, "Grain config applies position modulation and wrapping", "[grain]")
    {
        snapshot.inputNumSamples = 100;
        snapshot.startPositionSamples = 80;
        snapshot.spanSamples = 50;

        SECTION("No modulation")
        {
            grain.config(snapshot, 0.0f);
            REQUIRE(grain.getReadPosition() == 80.0f);
        }

        SECTION("Max modulation with wrapping")
        {
            grain.config(snapshot, 1.0f);
            REQUIRE(grain.getReadPosition() == 30.0f);
        }
    }

    // 2. WRAPPING
    TEST_CASE_METHOD(GrainFixture, "Grain advances read position based on speed", "[grain]")
    {
        snapshot.playback = 1.f;
        snapshot.speed = 1.5f;
        grain.config(snapshot, 0.0f);

        REQUIRE(grain.getReadPosition() == 0.0f);

        grain.nextReadPosition();
        REQUIRE(grain.getReadPosition() == 1.5f);

        grain.nextReadPosition();
        REQUIRE(grain.getReadPosition() == 3.0f);
    }

    TEST_CASE_METHOD(GrainFixture, "Grain wraps read position when reaching buffer end", "[grain]")
    {
        snapshot.inputNumSamples = 100;
        snapshot.startPositionSamples = 98;
        snapshot.speed = 1.5f;
        snapshot.playback = 1.f;
        grain.config(snapshot, 0.0f);

        REQUIRE(grain.getReadPosition() == 98.0f);

        grain.nextReadPosition();

        REQUIRE(grain.getReadPosition() == 99.5f);

        grain.nextReadPosition();

        REQUIRE(grain.getReadPosition() == 1.0f);
    }

    // 3. LIFECYCLE
    TEST_CASE_METHOD(GrainFixture, "Grain correctly reports exhaustion", "[grain]")
    {
        snapshot.durationSamples = 10;
        grain.config(snapshot, 0.0f);

        REQUIRE_FALSE(grain.isExhausted());

        for(int i = 0; i < 10; ++i)
        {
            grain.nextReadPosition();
        }

        // elapsedSamples == durationSamples
        REQUIRE(grain.isExhausted());
    }

    // 4. ENVELOPPE LUT
    TEST_CASE_METHOD(GrainFixture, "Grain envelope phase calculation", "[grain]")
    {
        snapshot.durationSamples = 100;
        snapshot.sustainRatio = 0.5f;
        grain.config(snapshot, 0.0f);

        SECTION("Start of grain is 0.0") { REQUIRE(grain.getPhase() == 0.0f); }

        SECTION("Mid Fade-in is 0.25")
        {
            for(int i = 0; i < 12; ++i)
                grain.nextReadPosition();
            REQUIRE(grain.getPhase() == 0.24f);
        }

        SECTION("End of Fade-in hits 0.5")
        {
            for(int i = 0; i < 25; ++i)
                grain.nextReadPosition();
            REQUIRE(grain.getPhase() == 0.5f);
        }

        SECTION("Sustain phase remains at 0.5")
        {
            for(int i = 0; i < 50; ++i)
                grain.nextReadPosition();
            REQUIRE(grain.getPhase() == 0.5f);
        }

        SECTION("Mid Fade-out is 0.75")
        {
            for(int i = 0; i < 87; ++i)
                grain.nextReadPosition(); // 75 + 12
            REQUIRE(grain.getPhase() == 0.74f);
        }

        SECTION("End of grain hits 1.0")
        {
            for(int i = 0; i < 100; ++i)
                grain.nextReadPosition();
            REQUIRE(grain.getPhase() == 1.0f);
        }
    }

    TEST_CASE_METHOD(GrainFixture, "Grain envelope phase calculation : no sustain", "[grain]")
    {
        snapshot.durationSamples = 100;
        snapshot.sustainRatio = 0.0f;
        grain.config(snapshot, 0.0f);

        for(int i = 0; i < 50; ++i)
            grain.nextReadPosition();

        REQUIRE(grain.getPhase() == 0.5f);

        for(int i = 0; i < 50; ++i)
            grain.nextReadPosition();

        REQUIRE(grain.getPhase() == 1.0f);
    }
}