#pragma once

#include "../../framework/Core.h"
#include "../../framework/Types.h"
#include "ProcessorFacade.h"

namespace particules
{
    class GrainVisualBuffer;
    class ParameterView;
    class EngineState;
    class UIState;
    struct UIContext
    {
        ValueTreeState& apvts;
        ParameterView& paramsView;
        EngineState& engineState;
        UIState& uiState;
        ProcessorFacade facade;
    };
}