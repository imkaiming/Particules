#pragma once

#include "../../framework/Core.h"

#include <juce_core/juce_core.h>

// math utilitary function
// expend this in the future
namespace particules
{

    inline float convertToPercentage(float toConvert) noexcept
    {
        return juce::jlimit(0.0f, 1.0f, toConvert * 0.01f); // 0 .. 100
    }

}