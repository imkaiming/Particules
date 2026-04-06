#pragma once

#include "../../framework/Core.h"
#include "../../gui/Component/slider/RotarySlider.h"

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>


namespace particules
{
    class VerticalMeterComponent : public juce::Component, private juce::Timer
    {
    public:
        VerticalMeterComponent(int numChannels = 2);

        void setLevel(int channel, float dbLevel);
        void setStereoLevels(float leftDB, float rightDB);

        void resized() override;
        void paint(juce::Graphics& g) override;

    private:
        void drawChannel(juce::Graphics& g, juce::Rectangle<int> area, int channelIndex);
        void timerCallback() override;

        int channels;
        std::vector<float> levels;
        std::vector<float> peaks;
        std::vector<int> peakCounters;
        juce::Array<juce::Rectangle<int>> channelAreas;

        TertiaryRotarySlider output;
    };
}