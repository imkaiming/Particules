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
        PingPongBuffer<VisualSnapshot> vb;
        GrainEnvelope lut;
        GrainHandle h;
        Grain g;
        PositionModulator posMod{};
        GrainPool pool;
        //VoiceManager vm;
        AudioBuffer inputBuffer{2, 48000};
        AudioBlock outputBlock{inputBuffer};
        ParameterSnapshot ps;
        //EngineSnapshot es;

        VoiceManagerFixture() //: vm{}
        {
            ps.durationSamples = 48000;
            ps.emission = 1;
            ps.linearGain = 0.707946f;
            ps.startPositionSamples = 24000;
            ps.selectionSamples = 12000;
            ps.speed = 1.f;
            ps.sustainRatio = 0.5f;
            ps.envMode = static_cast<EnvelopeMode>(1);
            ps.traversalMode = TraversalMode::Sine;
            ps.traversalFreq = 1.f;
            posMod.setSampleRate(48000);
        };
    };
    //TEST_CASE()
}