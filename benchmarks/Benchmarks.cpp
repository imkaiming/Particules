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
        std::cout << "[DSP TARGET] TIME BUDGET (100% CPU) = " << budgetMs << " ms\n";
        std::cout << "[DSP TARGET] 10% CPU Load = " << (budgetMs * 0.1) << " ms\n";
        std::cout << "========================================================\n";

        BENCHMARK_ADVANCED("Process block (Heavy Granular Load)")
        (Catch::Benchmark::Chronometer meter)
        {
            particules::ParticulesAudioProcessor processor;
            processor.setRateAndBufferSizeDetails(sampleRate, blockSize);
            processor.prepareToPlay(sampleRate, blockSize);

            juce::MidiBuffer midi;
            juce::AudioBuffer<float> buffer(numChannels, blockSize);

            for(int voice = 0; voice < particules::params::maxMidiVoice; voice++)
            {
                midi.addEvent(juce::MidiMessage::noteOn(1, 60 + voice, 1.0f), 0);
            }

            processor.getPluginCore().loadDebugPreset();

            juce::AudioProcessorValueTreeState& params = processor.getPluginCore().getAPVTS();

            if(auto* emissionParam = params.getParameter(particules::params::emission::id))
                emissionParam->setValueNotifyingHost(1.0f);
            if(auto* durationParam = params.getParameter(particules::params::duration::id))
                durationParam->setValueNotifyingHost(1.0f);

            // Spin-Wait to verify the audio file is loaded (asynchronous thread)
            bool isLoaded = false;
            for(int i = 0; i < 500; ++i)
            {
                buffer.clear();
                processor.processBlock(buffer, midi);
                if(buffer.getMagnitude(0, blockSize) > 0.0001f)
                {
                    isLoaded = true;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            REQUIRE(isLoaded == true);

            // 2. MESURE CATCH2
            meter.measure([&] {
                processor.processBlock(buffer, midi);
                doNotOptimize(buffer.getSample(0, 0));
            });
        
        };

        std::cout << "end audio processing benchmarks \n";
    }
}
