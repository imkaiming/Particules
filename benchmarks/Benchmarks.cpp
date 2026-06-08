#include <chrono>
#include <iostream>
#include <thread>

#include "framework/core/PluginParams.h"

// Helper cross-platform pour empêcher l'optimisation
template <typename T>
void doNotOptimize(T const& value)
{
#if defined(__clang__) || defined(__GNUC__) || defined(__APPLE__)
    asm volatile("" : : "g"(value) : "memory");
#else
    // Solution MSVC / Standard C++
    volatile const T* dummy = &value;
    (void)*dummy;
#endif
}

TEST_CASE("Boot performance")
{
    BENCHMARK_ADVANCED("Processor constructor")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<Catch::Benchmark::storage_for<particules::ParticulesAudioProcessor>> storage(size_t(meter.runs()));
        meter.measure([&](int i) { storage[(size_t)i].construct(); });
    };

    BENCHMARK_ADVANCED("Processor destructor")
    (Catch::Benchmark::Chronometer meter)
    {
        std::vector<Catch::Benchmark::destructable_object<particules::ParticulesAudioProcessor>> storage(size_t(meter.runs()));
        for(auto& s : storage)
            s.construct();
        meter.measure([&](int i) { storage[(size_t)i].destruct(); });
    };

    //BENCHMARK_ADVANCED("Editor open and close")
    //(Catch::Benchmark::Chronometer meter)
    //{
    //    // due to complex construction logic of the editor, let's measure open/close together
    //    meter.measure([&](int /* i */) {
    //        ParticulesAudioProcessor plugin;
    //        {
    //            auto editor = plugin.createEditorIfNeeded();
    //            //plugin.editorBeingDeleted (editor);
    //            delete editor;
    //        }
    //        return 0;
    //    });
    //};

#ifdef JUCE_LINUX
    BENCHMARK_ADVANCED("Editor open and close")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([](int) { return 0; });
    };
#else
    BENCHMARK_ADVANCED("Editor open and close")(Catch::Benchmark::Chronometer meter)
    {
        meter.measure([](int) {
            particules::ParticulesAudioProcessor plugin;

            juce::AudioProcessorEditor* editor = plugin.createEditorIfNeeded();

            plugin.editorBeingDeleted(editor);

            delete editor;

            return 0; // prevents optimization
        });
    };
#endif

    SECTION("Audio processing")
    {
        constexpr int blockSize = 512;
        constexpr double sampleRate = 48000.0;
        constexpr int numChannels = 2;

        const double budgetMs = (static_cast<double>(blockSize) / sampleRate) * 1000.0;

        std::cout << "\n========================================================\n";
        std::cout << "[DSP TARGET] Block Size: " << blockSize << " | Sample Rate: " << sampleRate << "Hz\n";
        std::cout << "[DSP TARGET] TIME BUDGET = " << budgetMs << " ms\n";
        std::cout << "========================================================\n";

        BENCHMARK_ADVANCED("Process block (Heavy Granular Load)")
        (Catch::Benchmark::Chronometer meter)
        {
            // 1. Processor setup (deterministic)
            particules::ParticulesAudioProcessor processor;
            processor.setRateAndBufferSizeDetails(sampleRate, blockSize);
            processor.prepareToPlay(sampleRate, blockSize);

            // 2. Test audio buffer (VALID size only)
            juce::AudioBuffer<float> buffer(numChannels, blockSize);
            buffer.clear();

            juce::Random rng(12345);
            for(int ch = 0; ch < buffer.getNumChannels(); ++ch)
            {
                float* data = buffer.getWritePointer(ch);

                for(int i = 0; i < buffer.getNumSamples(); ++i)
                    data[i] = rng.nextFloat() * 2.0f - 1.0f;
            }

            // 3. MIDI load (worst-case activation)
            juce::MidiBuffer midi;

            for(int voice = 0; voice < particules::params::maxMidiVoice; ++voice)
            {
                midi.addEvent(juce::MidiMessage::noteOn(1, 60 + voice, 1.0f), 0);
            }

            // 4. Parameter setup (deterministic state)
            auto& apvts = processor.getPluginCore().getAPVTS();

            if(auto* p = apvts.getParameter(particules::params::emission::id))
                p->setValueNotifyingHost(1.0f);

            if(auto* p = apvts.getParameter(particules::params::duration::id))
                p->setValueNotifyingHost(1.0f);

            if(auto* p = apvts.getParameter(particules::params::play::id))
                p->setValueNotifyingHost(1.0f);

            // 5. WARMUP (NOT measured)
            for(int i = 0; i < 100; ++i)
                processor.processBlock(buffer, midi);

            // 6. BENCHMARK (only measured section)
            meter.measure([&] {
                buffer.clear();
                processor.processBlock(buffer, midi);

                doNotOptimize(buffer.getSample(0, 0));
            });
        };

        std::cout << "end audio processing benchmarks\n";
    }
}
