//
//#include "../source/dsp/Grain.h"
//#include "../source/dsp/GrainPool.h"
//#include "../source/dsp/PositionModulator.h"
//#include "../source/dsp/VoiceManager.h"
//#include "../source/framework/Core.h"
//#include "../source/utils/GrainHandle.h"
//#include "../source/utils/ParameterSnapshot.h"
//#include "../source/framework/ParameterView.h"
//#include "../source/framework/PluginParams.h"
#include "../source/PluginProcessor.h"
#include <catch2/catch_test_macros.hpp>



namespace particulesTest
{
    using namespace particules;

    struct VoiceManagerFixture
    {
        GrainVisualBuffer vb;
        GrainEnvelope lut;
        GrainHandle h;
        Grain g;
        PositionModulator posMod{};
        GrainPool pool;
        VoiceManager vm;
        AudioBuffer inputBuffer{2, 48000};
        AudioBlock outputBlock{inputBuffer};
        ParameterSnapshot snapshot;

        VoiceManagerFixture() : vm{pool, posMod, lut, vb}
        {
            snapshot.durationSamples = 48000;
            snapshot.emission = 1;
            snapshot.linearGain = 0.707946f;
            snapshot.startPositionSamples = 24000;
            snapshot.selectionSamples = 12000;
            snapshot.speed = 1.f;
            snapshot.mix = 1.f;
            snapshot.sustainRatio = 0.5f;
            snapshot.envMode = static_cast<EnvelopeMode>(1);
            snapshot.traversalMode = TraversalMode::Sine;
            snapshot.traversalFreq = 1.f;
            snapshot.sampleRate = 48000.0;
            posMod.setSampleRate(snapshot.sampleRate);
        };
    };
    //TEST_CASE()
}