#pragma once

#include "framework/core/GuiTypes.h"

namespace particules
{
    class GrainVisualBuffer;
    class ParameterState;
    class AudioState;
    class UIState;
    class FromUI;
    //class FromAudio;
    struct UIContext
    {
        ValueTreeState& apvts;
        ParameterState& paramState;
        AudioState& audioState;
        UIState& uiState;
        FromUI& fui;
        //FromAudio& faudio;
    };
}