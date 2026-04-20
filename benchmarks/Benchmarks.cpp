#include <chrono>
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

        BENCHMARK_ADVANCED("Process block (maximum emission and duration)")
        (Catch::Benchmark::Chronometer meter)
        {
            // 1. Initialisation
            particules::ParticulesAudioProcessor processor;
            processor.setRateAndBufferSizeDetails(sampleRate, blockSize);
            processor.prepareToPlay(sampleRate, blockSize);

            juce::MidiBuffer midi;
            juce::AudioBuffer<float> buffer(numChannels, blockSize);

            // 2. asynchronous load
            processor.getPluginCore().loadDebugPreset();

            // 3. parameters settings
            juce::AudioProcessorValueTreeState& apvts = processor.getPluginCore().getAPVTS();
            if(auto* playParam = apvts.getParameter(particules::params::play::id))
                playParam->setValueNotifyingHost(1.0f);
            if(auto* emissionParam = apvts.getParameter(particules::params::emission::id))
                emissionParam->setValueNotifyingHost(1.0f);
            if(auto* durationParam = apvts.getParameter(particules::params::duration::id))
                durationParam->setValueNotifyingHost(1.0f);

            // 4. Spin-Wait
            bool isLoaded = false;
            for(int i = 0; i < 200; ++i)
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

            // 5. mesure the dsp
            // during offline benchmarks processblock is computed the faster possible
            // there is no strict sampleRate tempo to follow like in online rendering
            // in real context 48000 sample rate is 2.083e-5 seconds = 0.0208 ms
            // for a block size of 512 samples = 10.66 ms
            // so if the mesure is 1.06 ms well its means it requires 10% of the CPU load.
            // if its 5.33 ms it means its 50% which is way to much

            meter.measure([&] {
                processor.processBlock(buffer, midi);
                doNotOptimize(buffer.getSample(0, 0));
            });
        };
    }
}
