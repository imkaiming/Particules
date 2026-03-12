#include "../source/dsp/Grain.h"
#include "../source/dsp/GrainPool.h"
#include "../source/framework/PluginParams.h"
#include "../source/utils/GrainHandle.h"
#include <catch2/catch_test_macros.hpp>

/*
struct GrainHandle
{
    uint16_t index = 0xffff;
    uint16_t gen = 0;
    bool isValid() const { return index != 0xFFFF; };
    static GrainHandle getInvalidState() { return {}; }
};

*/

namespace audio_plugin_test
{
    struct GrainPoolFixture
    {
        GrainPool pool;
        Grain* grain = nullptr;
        GrainHandle handle;
    };

    TEST_CASE("1# GrainPool ctor", "[GrainPool]") { GrainPool g; }

    TEST_CASE_METHOD(GrainPoolFixture, "2# Acquire Grain", "[GrainPool]")
    {
        SECTION("acquiring one grain")
        {
            handle = pool.acquire();
            grain = pool.get(handle);
            REQUIRE(grain != nullptr);
            REQUIRE(grain->getGeneration() == 0);
            REQUIRE(grain->getActive());
        }

        SECTION("comparing two different grain")
        {
            GrainHandle h1 = pool.acquire();
            GrainHandle h2 = pool.acquire();
            REQUIRE(pool.get(h1) != pool.get(h2));
        }
    }

    TEST_CASE_METHOD(GrainPoolFixture, "3# Grain Release", "[GrainPool]")
    {
        SECTION("Can release a grain")
        {
            GrainHandle h0 = pool.acquire(); // G0, nextFree=1
            GrainHandle h1 = pool.acquire(); // G1, nextFree=2
            GrainHandle h2 = pool.acquire(); // G2, nextFree=3
            GrainHandle h3 = pool.acquire(); // G3, nextFree=4

            pool.release(h2);
            pool.release(h1);
            pool.release(h3);
            pool.release(h0);

            REQUIRE(pool.get(h0) == nullptr);
            REQUIRE(pool.get(h1) == nullptr);
            REQUIRE(pool.get(h2) == nullptr);
            REQUIRE(pool.get(h3) == nullptr);
        }

        SECTION("using the wrong gen silently fails")
        {
            GrainHandle h = pool.acquire();
            GrainHandle bad = h;
            bad.gen += 1;
            REQUIRE(pool.get(bad) == nullptr);
            REQUIRE_NOTHROW(pool.release(bad));
            REQUIRE(pool.get(h) != nullptr);
        }
        SECTION("releasing handle twice should do nothing")
        {
            GrainHandle h = pool.acquire();
            grain = pool.get(h);
            REQUIRE(grain != nullptr);
            pool.release(h);
            REQUIRE(grain->getGeneration() == 1);
            pool.release(h);
            REQUIRE(grain->getGeneration() == 1);
            REQUIRE_FALSE(grain->getActive());
        }

        SECTION("Released handles are reused in LIFO order")
        {
            GrainHandle h1 = pool.acquire();
            GrainHandle h2 = pool.acquire();

            pool.release(h2);
            auto h3 = pool.acquire();

            REQUIRE(h3.index == h2.index);
            REQUIRE(h3.gen != h2.gen);
        }
    }

    TEST_CASE_METHOD(GrainPoolFixture, "4# Grain Pool Capacity Saturation", "[GrainPool]")
    {
        SECTION("Can acquire up to capacity")
        {
            for(size_t i = 0; i < Param::MaxGrains; ++i)
            {
                GrainHandle h = pool.acquire();
                REQUIRE(h.isValid());
            }

            handle = pool.acquire();
            REQUIRE_FALSE(handle.isValid());
            grain = pool.get(handle);
            REQUIRE(grain == nullptr);
        }

        SECTION("Can repeat after a full reset")
        {
            pool.reset();

            for(size_t i = 0; i < Param::MaxGrains; ++i)
            {
                GrainHandle h = pool.acquire();
                REQUIRE(h.isValid());
            }

            handle = pool.acquire();
            REQUIRE_FALSE(handle.isValid());
            grain = pool.get(handle);
            REQUIRE(grain == nullptr);
        }
    }

    TEST_CASE_METHOD(GrainPoolFixture, "5# Grain Handle Generation", "[GrainPool]")
    {
        SECTION("Generation increments on release")
        {
            GrainHandle h = pool.acquire();
            REQUIRE(h.gen == 0);
            pool.release(h);
            h = pool.acquire();
            REQUIRE(h.gen == 1);
            pool.release(h);
            h = pool.acquire();
            REQUIRE(h.gen == 2);
            pool.release(h);
            h = pool.acquire();
            REQUIRE(h.gen == 3);
        }
        SECTION("generation resett with reset()")
        {
            GrainHandle h = pool.acquire();
            grain = pool.get(h);
            REQUIRE(grain->getGeneration() == 0);
            pool.reset();
            h = pool.acquire();
            grain = pool.get(h);
            REQUIRE(grain->getGeneration() == 0);
        }
    }
}