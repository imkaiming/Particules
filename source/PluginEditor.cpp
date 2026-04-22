#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace particules
{
    ParticulesAudioProcessorEditor::ParticulesAudioProcessorEditor(ParticulesAudioProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p), mainPanel(p.getUIContext()),
          keyboardComponent{p.getPluginCore().getKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard}
    {
        setLookAndFeel(&lnf);

        setResizable(true, true);
        setResizeLimits(gui::windowWidthMin, gui::windowHeightMin, gui::windowWidthMax, gui::windowHeightMax);

        const float ratio = 1.5f; // ratio 3:2
        getConstrainer()->setFixedAspectRatio(ratio);

        setSize(gui::windowWidthInit, gui::windowHeightInit);

        addAndMakeVisible(&mainPanel);
        addAndMakeVisible(&keyboardComponent);
        setWantsKeyboardFocus(true);
        keyboardComponent.setWantsKeyboardFocus(true);

#if ENABLE_DEBUG_PRESET
        pluginProcessor.loadDebugPreset();
#endif

#if ENABLE_MELATONINE_INSPECTOR
        inspector.setVisible(true);
        inspector.toggle(true);
#endif
    }

    ParticulesAudioProcessorEditor::~ParticulesAudioProcessorEditor() { setLookAndFeel(nullptr); }

    void ParticulesAudioProcessorEditor::resized()
    {
        //mainPanel.setBounds(getLocalBounds());

        // temp
        juce::Rectangle<int> bounds = getLocalBounds();
        const int pianoHeight = 60;
        keyboardComponent.setBounds(bounds.removeFromBottom(pianoHeight));
        mainPanel.setBounds(bounds);
    }
}