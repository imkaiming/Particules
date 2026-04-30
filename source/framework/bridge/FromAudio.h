#pragma once

#include <juce_core/juce_core.h>

#include "framework/bridge/PingPongBuffer.h"
#include "framework/state/AudioState.h"
#include "framework/core/Core.h"
#include "utils/struct/VisualSnapshot.h"

/**
 * @class FromAudio
 * @brief Unidirectional telemetry bridge (Audio Thread -> UI / State).
 *
 * This class allows the real-time audio thread to export telemetry data and 
 * rendering snapshots (e.g., grain visuals) to the graphical interface. It 
 * operates strictly in a Producer role, writing data into concurrent, 
 * thread-safe structures like the PingPongBuffer.
 * 
 * [CRITICAL]: All methods within this class are executed on the high-priority 
 * 
 * Audio Thread. Consequently, every implementation must be strictly lock-free, 
 * wait-free, and must not perform any memory allocations or system calls to 
 * prevent audio dropouts.
 */
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

        void setLink(bool b) noexcept { audioState.setLink(b); }

        FromAudio(const FromAudio&) = delete;
        FromAudio& operator=(const FromAudio&) = delete;
        FromAudio(FromAudio&&) = delete;    
        FromAudio& operator=(FromAudio&&) = delete;

    private:
        AudioState& audioState;
        PingPongBuffer<VisualSnapshot>& visualBuffer;
    };

}