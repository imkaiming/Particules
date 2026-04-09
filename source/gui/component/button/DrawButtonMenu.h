#pragma once

#include "../../../framework/GuiTypes.h"
#include "../../../framework/PluginParams.h"
#include "../../../framework/Core.h"
#include "../../lookandfeelv2/Colours.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{

    class DrawButtonMenu : public juce::Component
    {
    public:
        DrawButtonMenu();

        void resized() override;

        void paint(juce::Graphics& g) override;

        void mouseDown(const juce::MouseEvent& /*e*/) override { showPopupMenu(); }
        virtual juce::Path createCurvePath(juce::Rectangle<float> bounds) = 0;
        virtual void showPopupMenu() = 0;

        // Override this to create menu icons
        virtual juce::Image createMenuIcon(int itemIndex) = 0;

    protected:
        juce::Colour curveColour = coloursv2::cyan;
        juce::Rectangle<float> buttonBounds;

        juce::Path buildPathFromFunction(
            juce::Rectangle<float> bounds, std::function<float(float)> evalFunc, int numPoints = 100);

    private:

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawButtonMenu)
    };
}
