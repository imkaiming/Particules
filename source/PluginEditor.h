/**
 * This class is the graphical user interface for the plugin and is responsible
 * for displaying the controls and parameters.
*/

#pragma once

#include "PluginProcessor.h"
#include "gui/panel/MainPanel.h"
#include "utils/CustomLookAndFeel.h"

#include "melatonin_inspector/melatonin_inspector.h"

namespace particules
{
    class ParticulesAudioProcessorEditor : public juce::AudioProcessorEditor
    {
    public:
        ParticulesAudioProcessorEditor(ParticulesAudioProcessor&);
        ~ParticulesAudioProcessorEditor() override;
        void paint(juce::Graphics&) override;
        void resized() override;

        CustomLookAndFeel& getCustomLook() { return customLookAndFeel; };
        const MainPanel& getMainPanel() const noexcept { return mainPanel; };

    private:
        ParticulesAudioProcessor& pluginProcessor;
        MainPanel mainPanel;
        CustomLookAndFeel& customLookAndFeel;
        int width, heigth;

#if ENABLE_MELATONINE_INSPECTOR
        melatonin::Inspector inspector{*this};
#endif

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessorEditor)
    };
}