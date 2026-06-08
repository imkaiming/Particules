#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

#include "framework/core/PluginParams.h"
#include "PluginProcessor.h"

static inline void doNotOptimize(const float& v)
{
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "g"(v) : "memory");
#else
    volatile float sink = v;
    (void)sink;
#endif
}

TEST_CASE("DSP - GrainProcessor performance (steady state)", "[dsp][grain]")
{
    constexpr int blockSize = 512;
    constexpr double sampleRate = 48000.0;
    constexpr int numChannels = 2;

    juce::AudioBuffer<float> buffer(numChannels, blockSize);
    juce::MidiBuffer midi;

    // ------------------------------------------------------------
    // Deterministic input signal (NO OOB BUG)
    // ------------------------------------------------------------
    juce::Random rng(12345);

    for (int ch = 0; ch < numChannels; ++ch)
    {
        float* data = buffer.getWritePointer(ch);

        for (int i = 0; i < blockSize; ++i)
            data[i] = rng.nextFloat() * 2.0f - 1.0f;
    }

    // ------------------------------------------------------------
    // MIDI stress configuration (fixed worst-case voice load)
    // ------------------------------------------------------------
    for (int v = 0; v < particules::params::maxMidiVoice; ++v)
        midi.addEvent(juce::MidiMessage::noteOn(1, 60 + v, 1.0f), 0);

    // ------------------------------------------------------------
    // Processor setup
    // ------------------------------------------------------------
    particules::ParticulesAudioProcessor processor;
    processor.setRateAndBufferSizeDetails(sampleRate, blockSize);
    processor.prepareToPlay(sampleRate, blockSize);

    processor.getPluginCore().loadDebugPreset();

    // ------------------------------------------------------------
    // STABILISATION PHASE (critical for correct statistics)
    // ------------------------------------------------------------
    SECTION("Warmup stabilisation")
    {
        for (int i = 0; i < 300; ++i)
        {
            buffer.clear();
            processor.processBlock(buffer, midi);
        }

        REQUIRE(true);
    }

    // ------------------------------------------------------------
    // STEADY STATE BENCHMARK
    // ------------------------------------------------------------
    SECTION("ProcessBlock steady state cost")
    {
        // Ensure deterministic state BEFORE measurement
        for (int i = 0; i < 200; ++i)
        {
            buffer.clear();
            processor.processBlock(buffer, midi);
        }

        BENCHMARK_ADVANCED("processBlock (steady state)")
        (Catch::Benchmark::Chronometer meter)
        {
            meter.measure([&] {
                buffer.clear();
                processor.processBlock(buffer, midi);

                doNotOptimize(buffer.getSample(0, 0));
            });
        };
    }
}
