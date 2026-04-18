#pragma once

#include <juce_core/juce_core.h>

#include "framework/bridge/PingPongBuffer.h"
#include "framework/state/AudioState.h"
#include "framework/core/Core.h"
#include "utils/struct/VisualSnapshot.h"

namespace particules
{
    class FromAudio
    {
    public:
        FromAudio(AudioState& as, PingPongBuffer<VisualSnapshot>& vb) : audioState{as}, visualBuffer{vb} {}
        ~FromAudio() = default;

        void setNumActiveGrains(int num) const noexcept { audioState.setNumActiveGrains(num); }
        VisualSnapshot& beginWriteVisualSnapshot() const noexcept { return visualBuffer.beginWriteBuffer(); }
        void endWriteVisualSnapshot() const noexcept { visualBuffer.endWriteBuffer(); }

        FromAudio(const FromAudio&) = delete;
        FromAudio& operator=(const FromAudio&) = delete;
        FromAudio(FromAudio&&) = delete;    
        FromAudio& operator=(FromAudio&&) = delete;

    private:
        AudioState& audioState;
        PingPongBuffer<VisualSnapshot>& visualBuffer;
    };

}