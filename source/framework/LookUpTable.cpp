#include "LookUpTable.h"

namespace particules
{
    const float LookUpTable::getValue(float phase) const noexcept
    {
        const float p = (phase < 0.f) ? 0.f : (phase > 1.f ? 1.f : phase);
        const float x = p * (static_cast<float>(SIZE) - 1.f);
        const int i = std::min(static_cast<int>(x), SIZE - 2);

        const float frac = x - i;
        const float a = table[i];
        const float b = table[i + 1];

        return lerp(a, b, frac);
    }

}