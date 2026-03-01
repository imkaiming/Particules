/*
  ==============================================================================

    StatusBar.h
    Created: 4 Feb 2023 1:26:27pm
    Author:  user

  ==============================================================================
*/
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

struct UIContext;
class ParticulesAudioProcessor;
class StatusBarFrame : public juce::Component, public juce::Timer
{
public:
    StatusBarFrame(UIContext& uic);
    ~StatusBarFrame() = default;
    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    juce::Label statusLabel;
    UIContext& uic;
    ParticulesAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StatusBarFrame)
};