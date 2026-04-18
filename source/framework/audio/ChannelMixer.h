#pragma once

#include <juce_core/juce_core.h>
//#include <juce_audio_basics/juce_audio_basics.h>

#include "framework/core/PluginTypes.h"

// just convert a buffer into a downmixed buffer
// apply predefined mix matrix
// offline

namespace particules
{
    class ChannelMixer
    {
    public:
        ChannelMixer() = default;
        ~ChannelMixer() = default;

        AudioBuffer downmix(const AudioBuffer& inputBuffer) const;
        void setTargetChannel(int) noexcept;

    private:
        static constexpr int MAX_CHANNELS = 8;
        int targetChannels = 0;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelMixer)
    };
}