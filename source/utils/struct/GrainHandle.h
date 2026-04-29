#pragma once
namespace particules
{
    struct GrainHandle
    {
        uint16_t index = 0xFFFF;
        uint16_t gen = 0; // this solve the Stale Handle problem (enable safe reuse)
        GrainHandle(uint16_t i, uint16_t g) : index{i}, gen{g} {}
        GrainHandle() = default;
        bool isValid() const noexcept { return index != 0xFFFF; }
        static GrainHandle getInvalidState() noexcept { return {0xFFFF, 0}; }
    };
}