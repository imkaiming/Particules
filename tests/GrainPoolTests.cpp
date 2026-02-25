#include "../source/dsp/Grain.h"
#include "../source/dsp/GrainPool.h"
#include "../source/framework/GrainHandle.h"
#include <catch2/catch_test_macros.hpp>

/*
	Grain* get(const GrainHandle handle) ;
	bool isValid(const GrainHandle handle) const ;
	bool acquire(GrainHandle& outHandle, Grain*& outGrain);
	void release(const GrainHandle hadle);
	void reset();


struct GrainHandle
{
    uint16_t index = 0xffff;
    uint16_t gen = 0;
    bool isValid() const { return index != 0xFFFF; };
    static GrainHandle getInvalidState() { return {}; }
};

*/

struct GrainPoolFixture
{
    GrainPool pool;
    Grain* grain = nullptr;
    GrainHandle handle;
};

namespace audio_plugin_test
{
    TEST_CASE("1# GrainPool ctor", "[GrainPool]")
    {
        GrainPool* g;
        try
        {
            g = new GrainPool();
            delete g;
        } catch(...)
        {
            REQUIRE(false);
        }

        REQUIRE(true);
    }

    TEST_CASE("2# get a grain", "[GrainPool]")
    {
        GrainPoolFixture f;
        f.handle.index = 0;
        f.handle.gen = 0;

        try
        {
            f.pool.acquire(f.handle, f.grain);
        } catch(const std::exception& e)
        {
            printf("Exception: %s\n", e.what());
        }

        REQUIRE(f.grain != nullptr);
    }

    //TEST_CASE(GrainPoolFixture, "3#")
}