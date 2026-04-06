#pragma once

#include "../../framework/Core.h"

// math utilitary function
// expend this in the future
namespace particules
{
    inline float convertToPercentage(float toConvert) noexcept { return std::clamp(toConvert * 0.01f, 0.0f, 1.0f); }
}