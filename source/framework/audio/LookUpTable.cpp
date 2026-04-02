#include "LookUpTable.h"

namespace particules
{
    LookUpTable::LookUpTable() : table{} {}

    const float LookUpTable::getValue(float phase) const noexcept
    {
        const float p = (phase < 0.f) ? 0.f : (phase > 1.f ? 1.f : phase); // clamping without std::clamp
        const float x = p * (static_cast<float>(SIZE) - 1.f);

        const int i1 = static_cast<int>(x);
        const int i2 = (i1 < SIZE - 1) ? (i1 + 1) : i1; // avoid out of bounds

        const float frac = x - i1;

        const float a = table[i1];
        const float b = table[i2];

        return lerp(a, b, frac);
    }

}