#pragma once

#include "../../framework/GuiTypes.h"

namespace particules
{
    class GrainVisualBuffer;
    class ParameterView;
    class AudioState;
    class UIState;
    struct ProcessorFacade; 
    struct UIContext
    {
        ValueTreeState& apvts;
        ParameterView& paramsView;
        AudioState& audioState;
        UIState& uiState;
        ProcessorFacade& facade;
    };
}