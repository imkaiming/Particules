#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace particules
{
    ParticulesAudioProcessorEditor::ParticulesAudioProcessorEditor(ParticulesAudioProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p), mainPanel(p.getUIContext()),
          globalLookAndFeel()
    {
        setLookAndFeel(&globalLookAndFeel);

        setResizable(true, true);
        setResizeLimits(windowWidthMin, windowHeightMin, windowWidthMax, windowHeightMax);

        const float ratio = 1.5f; // ratio 3:2
        getConstrainer()->setFixedAspectRatio(ratio);

        setSize(windowWidthInit, windowHeightInit);

        addAndMakeVisible(&mainPanel);
#if ENABLE_DEBUG_PRESET
        pluginProcessor.loadDebugPreset();
#endif

#if ENABLE_MELATONINE_INSPECTOR
        inspector.setVisible(true);
        inspector.toggle(true);
#endif
    }

    ParticulesAudioProcessorEditor::~ParticulesAudioProcessorEditor() { setLookAndFeel(nullptr); }

    void ParticulesAudioProcessorEditor::paint(juce::Graphics& g) { /* g.fillAll(colours::black); */ }

    void ParticulesAudioProcessorEditor::resized() { mainPanel.setBounds(getLocalBounds()); }
}