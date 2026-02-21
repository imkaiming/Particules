/*
  ==============================================================================

    StatusBar.h
    Created: 4 Feb 2023 1:26:27pm
    Author:  user

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>


class StatusBarFrame : public juce::Component {
public:
    StatusBarFrame();//UIContext& uic);
    ~StatusBarFrame() = default;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Label statusLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarFrame)
};