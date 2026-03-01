
#include "../source/dsp/Grain.h"
#include "../source/dsp/GrainPool.h"
#include "../source/dsp/VoiceManager.h"
#include "../source/framework/Core.h"
#include "../source/framework/GrainHandle.h"
#include "../source/framework/ParameterSnapshot.h"
#include "../source/framework/ParameterView.h"
#include "../source/framework/ParamsId.h"
#include <catch2/catch_test_macros.hpp>

struct VoiceManagerFixture
{
    GrainHandle h;
    Grain g;
    GrainPool pool;
    VoiceManager vm;
    AudioBuffer inputBuffer{2, 48000};
    AudioBlock outputBlock{inputBuffer};
    ParameterSnapshot snapshot;

    VoiceManagerFixture() : vm{pool}
    {
        snapshot.durationSample = 48000;
        snapshot.density = 1;
        snapshot.linearGain = 0.707946f;
        snapshot.startPositionSample = 24000;
        snapshot.selectionSample = 12000;
        snapshot.speed = 1.f;
        snapshot.mix = 1.f;
        snapshot.sustainRatio = 0.5f;
        snapshot.envType = 1;
        snapshot.traversalMode = 1;
        snapshot.traversalTime = 0;
        snapshot.sampleRate = 48000.0;
    };
};

namespace audio_plugin_test
{
    //TEST_CASE()
}