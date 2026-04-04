#include "VerticalMeterComponent.h"

#include "../lookandfeelv2/Colours.h"

namespace particules
{

    VerticalMeterComponent::VerticalMeterComponent(int numChannels) : channels(numChannels)
    {
        levels.resize(numChannels, -60.0f);
        peaks.resize(numChannels, -60.0f);
        peakCounters.resize(numChannels, 0);

        addAndMakeVisible(output);
        output.setRange(-60.0, 12.0, 0.1);
        output.setValue(0.0);

        startTimerHz(30);
    }

    void VerticalMeterComponent::setStereoLevels(float leftDB, float rightDB)
    {
        setLevel(0, leftDB);
        setLevel(1, rightDB);
    }

    void VerticalMeterComponent::setLevel(int channel, float dbLevel)
    {
        if(juce::isPositiveAndBelow(channel, (int)levels.size()))
        {
            levels[channel] = dbLevel;
            if(dbLevel > peaks[channel])
            {
                peaks[channel] = dbLevel;
                peakCounters[channel] = 20;
            }
        }
    }

    void VerticalMeterComponent::resized()
    {
        auto bounds = getLocalBounds().reduced(4);

        auto footArea = bounds.removeFromBottom(55);
        auto meterArea = bounds; 

        output.setBounds(footArea.withSizeKeepingCentre(42, 42));

        channelAreas.clear();
        int chWidth = meterArea.getWidth() / channels;
        for(int i = 0; i < channels; ++i)
            channelAreas.add(meterArea.removeFromLeft(chWidth));
    }

    void VerticalMeterComponent::paint(juce::Graphics& g)
    {
        auto bounds = getLocalBounds().reduced(4);
        g.setColour(colours::blackest);
        g.fillRoundedRectangle(bounds.toFloat(), 6.0f);

        for(int i = 0; i < channels; ++i)
            if(i < channelAreas.size())
                drawChannel(g, channelAreas[i], i);
    }

    void VerticalMeterComponent::drawChannel(juce::Graphics& g, juce::Rectangle<int> area, int channelIndex)
    {
        if(area.getHeight() < 20)
            return;

        auto labelArea = area.removeFromTop(15);
        g.setColour(juce::Colours::grey.withAlpha(0.8f));
        g.setFont(9.0f);
        juce::String labelText = (channelIndex == 0) ? "L" : (channelIndex == 1 ? "R" : juce::String(channelIndex + 1));
        g.drawText(labelText, labelArea, juce::Justification::centred);

        const float circleSize = 6.0f;
        const float spacing = 4.0f;

        const float groundY = area.getBottom();
        const float ceilingY = area.getY() + spacing;

        const float availableHeight = groundY - ceilingY;
        if(availableHeight < circleSize)
            return;

        int numLEDs = static_cast<int>((availableHeight + spacing) / (circleSize + spacing));
        numLEDs = juce::jlimit(4, 50, numLEDs);

        const float x = area.getCentreX() - circleSize / 2.0f;
        const float level = levels[channelIndex];
        const float peak = peaks[channelIndex];

        for(int i = 0; i < numLEDs; ++i)
        {
            float y = groundY - (i + 1) * circleSize - i * spacing;

            float thresholdDB = juce::jmap((float)i, 0.0f, (float)(numLEDs - 1), -60.0f, 6.0f);

            juce::Colour ledColour;
            bool isLit = level >= thresholdDB;
            bool isPeak = peak >= thresholdDB && peakCounters[channelIndex] > 0;

            if(isPeak && i >= numLEDs - 2)
                ledColour = juce::Colour(0xFFFF6B35);
            else if(isLit)
            {
                if(i >= numLEDs - 3)
                    ledColour = juce::Colour(0xFFFFD700).brighter(0.2f);
                else
                    ledColour = juce::Colours::white.withAlpha(0.9f);
            }
            else
            {
                ledColour = juce::Colour(0xFF2A2A2A);
            }

            g.setColour(ledColour);
            g.fillEllipse(x, y, circleSize, circleSize);

            if(isLit || (isPeak && i >= numLEDs - 2))
            {
                g.setColour(juce::Colours::white.withAlpha(0.5f));
                g.fillEllipse(x + circleSize * 0.25f, y + circleSize * 0.15f, circleSize * 0.3f, circleSize * 0.3f);
            }
        }
    }

    void VerticalMeterComponent::timerCallback()
    {
        for(size_t i = 0; i < peakCounters.size(); ++i)
        {
            if(peakCounters[i] > 0)
                peakCounters[i]--;
            else
                peaks[i] = peaks[i] * 0.9f + levels[i] * 0.1f;
        }
        repaint();
    }
}