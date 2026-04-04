#pragma once

#include "../../framework/GuiTypes.h"

namespace particules
{
    class GrainVisualBuffer;
    class ParameterView;
    class EngineState;
    class UIState;
    class ProcessorFacade; 
    struct UIContext
    {
        ValueTreeState& apvts;
        ParameterView& paramsView;
        EngineState& engineState;
        UIState& uiState;
        ProcessorFacade& facade;
    };
}