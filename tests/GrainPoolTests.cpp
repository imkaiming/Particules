#include "../source/dsp/Grain.h"
#include "../source/dsp/GrainPool.h"
#include "../source/framework/GrainHandle.h"
#include "../source/framework/ParamsId.h"
#include <catch2/catch_test_macros.hpp>

/*
	Grain* get(const GrainHandle handle) ;
	bool isValid(const GrainHandle handle) const ;
	bool acquire(GrainHandle& outHandle, Grain*& outGrain);
	void release(const GrainHandle hadle);
	void reset();
*/
/*
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
        g = new GrainPool();
        delete g;
    }

    TEST_CASE_METHOD(GrainPoolFixture, "2# acquire provide a valid grain", "[GrainPool]")
    {
        handle.index = 0;
        handle.gen = 0;

        REQUIRE(grain == nullptr);
        REQUIRE(pool.acquire(handle, grain));
        REQUIRE_FALSE(grain == nullptr);
        REQUIRE(pool.isValid(handle));
    }

    TEST_CASE_METHOD(GrainPoolFixture, "3# release ", "[GrainPool]")
    {
        //handle.index = 0;
        //pool.acquire(handle, grain);
        //REQUIRE(grain != nullptr);
    }

    TEST_CASE_METHOD(GrainPoolFixture, "3# handle at index -1 should acquire grain", "[GrainPool]")
    {
        //handle.index = -1;
        //REQUIRE(!pool.acquire(handle, grain));
        //REQUIRE(!pool.isValid(handle));
    }

    TEST_CASE_METHOD(GrainPoolFixture, "4# acquire fails when pool is empty even after reset", "[GrainPool]")
    {
        for(size_t i = 0; i < Param::MaxGrains; ++i)
            REQUIRE(pool.acquire(handle, grain));

        REQUIRE_FALSE(pool.acquire(handle, grain));

        pool.reset();

        for(size_t i = 0; i < Param::MaxGrains; ++i)
            REQUIRE(pool.acquire(handle, grain));

        REQUIRE_FALSE(pool.acquire(handle, grain));
    }
}