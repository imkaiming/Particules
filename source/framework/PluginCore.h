#pragma once

#include "framework/Core.h"
#include "framework/bridge/AudioState.h"
#include "framework/bridge/PingPongBuffer.h"
#include "framework/bridge/RingBuffer.h"
#include "framework/bridge/UIState.h"
#include "utils/struct/AudioPayload.h"

namespace particules
{
    class PluginCore
    {
    public:
        PluginCore() : synchronizer(currentPayload, garbageQueue, audioState, uiState) { synchronizer.start(10); }
        ~PluginCore() = default;

    private:
        AudioState audioState;
        UIState uiState;

        RingBuffer<AudioPayload> incomingQueue;
        RingBuffer<AudioPayload> garbageQueue;
        std::atomic<AudioPayload*> currentPayload{nullptr};

        StateSynchronizer synchronizer; // Le nouveau gestionnaire autonome
    };
}