#pragma once

#include "../../framework/Constants.h"
//#include "../../framework/bridge/EngineState.h"
#include "../../framework/bridge/UIState.h"
#include "../../utils/MyColours.h"
#include "../../utils/struct/UIContext.h"

/**
* On a besoin de la valeur de la position du grain dans le buffer pour montrer sa position sur l'axe x.
* et on a besoin de la valeur de son relative à enveloppe pour la peindre sa transparence.
* Plus un son est fort plus il est opaque.
* Sur l'axe Y, les valeurs seront disposés aléatoirement.
*
* La classe GrainVisualComponent est appelé avec update à chaque lecture de sample ou à chaque chunk de buffer
*/

namespace particules
{

    class GrainVisualComponent : public juce::Component, private juce::Timer
    {
    public:
        GrainVisualComponent(UIContext&);
        ~GrainVisualComponent() override;

        void paint(juce::Graphics& g) override;
        void resized() override;

        void setNumSamples(int);

    private:
        static constexpr int MAXGRAINS = static_cast<int>(global::maxGrains);
        static constexpr float GSIZE = UI::grainVisualSize;
        static constexpr float GCENTER = GSIZE / 2.f;
        void timerCallback() override;

        juce::Colour colour;
        UIState& uiState;
        //ParameterView& parameterView;
        int numSamples;

        float invWidthSamples;
    };
}