#pragma once 

namespace particules
{
    template <typename T>
    T map(T value, T inMin, T inMax, T outMin, T outMax)
    {
        return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
    }

}