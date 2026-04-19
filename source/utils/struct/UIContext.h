#pragma once

#include "framework/core/GuiTypes.h"

namespace particules
{
    class GrainVisualBuffer;
    class ParameterState;
    class AudioState;
    class UIState;
    class FromUI;
    class FromAudio;
    struct UIContext
    {
        ValueTreeState& apvts;
        ParameterState& paramState;
        const AudioState& audioState;
        /*const*/ UIState& uiState; // cannot make it const because of the add listener
        FromUI& fui;
        FromAudio& faudio;
    };
}