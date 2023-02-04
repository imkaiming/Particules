/*
  ==============================================================================

    StatusBar.h
    Created: 4 Feb 2023 1:26:27pm
    Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>
#pragma once

class StatusBarFrame : public juce::Component {
public:
    StatusBarFrame();
    ~StatusBarFrame();
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarFrame)
};