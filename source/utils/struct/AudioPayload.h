#pragma once

#include <memory>

#include "framework/core/PluginTypes.h"

namespace particules
{
    struct AudioPayload
    {
        std::unique_ptr<AudioBuffer> buffer;
        int numSamples = 0;
        int numChannels = 0;
        juce::File file;
    };
}