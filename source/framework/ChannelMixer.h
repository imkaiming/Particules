#pragma once

#include "Types.h"

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

        //static AudioBuffer downmix(const AudioBuffer& input, const int outputNumChannels);
        AudioBuffer downmix(const AudioBuffer& inputBuffer) const;
        void setTargetChannel(int) noexcept;

    private:
        static constexpr int MAX_CHANNELS = 8;
        int targetChannels = 0;
        //static void computeMixMatrix(int inputChannels, int outputChannels, float* matrix, int maxChannels) noexcept;
        //static void applyMix(
        //    const AudioBuffer& input, AudioBuffer& output, const float* matrix, int inputChannels, int outputChannels) noexcept;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChannelMixer)
    };
}