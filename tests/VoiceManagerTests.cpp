
#include "../source/dsp/Grain.h"
#include "../source/dsp/GrainPool.h"
#include "../source/dsp/VoiceManager.h"
#include "../source/framework/GrainHandle.h"
#include "../source/framework/ParamsId.h"
#include <catch2/catch_test_macros.hpp>

struct VoiceManagerFixture
{
    VoiceManager vm;
    GrainHandle h;
    Grain g;
};

namespace audio_plugin_test
{
    //TEST_CASE()
}