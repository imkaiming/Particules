#pragma once

#include "../../../framework/Core.h"
#include "../../../framework/GuiTypes.h"
#include "../../../framework/PluginParams.h"
#include "../../lookandfeelv2/Colours.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{

    class SliderButtonMenu : public juce::Component
    {
    public:
        SliderButtonMenu(const str& name);

        void resized() override;
        void paint(juce::Graphics& g) override;

        void mouseDown(const juce::MouseEvent& e) override;
        void mouseDrag(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent& e) override;

        virtual juce::Path createCurvePath(juce::Rectangle<float> bounds) = 0;
        virtual void showPopupMenu() = 0;
        virtual juce::Image createMenuIcon(int itemIndex) = 0;

        virtual float getDragValue() const { return 0.0f; }
        virtual void setDragValue(float newValue) {}

    protected:
        juce::Colour curveColour = coloursv2::cyan;
        juce::Rectangle<float> buttonBounds;

        juce::Path buildPathFromFunction(
            juce::Rectangle<float> bounds, std::function<float(float)> evalFunc, int numPoints = 100);
        juce::Label nameLabel;

    private:
        bool isDragging = false;
        juce::Point<int> mouseDownPosition;
        float valueOnMouseDown = 0.0f;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliderButtonMenu)
    };
}
