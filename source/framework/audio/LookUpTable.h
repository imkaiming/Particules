#pragma once

#include "framework/core/PluginParams.h"
#include "framework/core/Core.h"

namespace particules
{
    class LookUpTable
    {
    public:
        LookUpTable();
        ~LookUpTable() = default;

        const float getValue(float phase) const noexcept;

        template <typename Fn>
        void populate(Fn&& fn)
        {
            fn(std::span<float>(table));
        }

    private: // make this protected if you want to specialized the LUT
        static constexpr int SIZE = params::maxLutSize;
        std::array<float, SIZE> table;

        //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookUpTable)
    };
}