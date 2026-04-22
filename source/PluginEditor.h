/**
 * This class is the graphical user interface for the plugin and is responsible
 * for displaying the controls and parameters.
*/

#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "melatonin_inspector/melatonin_inspector.h"

#include "gui/lookandfeel/GlobalLookAndFeel.h"
#include "gui/lookandfeelv2/MainLNF.h"
#include "gui/panel/MainPanel.h"

namespace particules
{
    class ParticulesAudioProcessor;
    class ParticulesAudioProcessorEditor : public juce::AudioProcessorEditor
    {
    public:
        ParticulesAudioProcessorEditor(ParticulesAudioProcessor&);
        ~ParticulesAudioProcessorEditor() override;

        //void paint(juce::Graphics&) override;
        void resized() override;

        //GlobalLookAndFeel& getCustomLook() { return globalLookAndFeel; };

    private:
        ParticulesAudioProcessor& pluginProcessor;
        MainLNF lnf;
        MainPanel mainPanel;
        juce::MidiKeyboardComponent keyboardComponent;

#if ENABLE_MELATONINE_INSPECTOR
        melatonin::Inspector inspector{*this};
#endif

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessorEditor)
    };
}