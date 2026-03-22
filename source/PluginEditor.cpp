#include "PluginEditor.h"
#include "PluginProcessor.h"

namespace particules
{
    ParticulesAudioProcessorEditor::ParticulesAudioProcessorEditor(ParticulesAudioProcessor& p)
        : AudioProcessorEditor(&p), pluginProcessor(p), mainFrame(p.getUIContext()),
          customLookAndFeel(p.getUIContext().customLookAndFeel)
    {
        setLookAndFeel(&customLookAndFeel);

        setResizable(true, true);
        setResizeLimits(450, 225, 1200, 600);

        const float ratio = 2.f;
        getConstrainer()->setFixedAspectRatio(ratio);

        width = 700;
        heigth = 350;
        setSize(width, heigth);

        addAndMakeVisible(&mainFrame);

#if ENABLE_DEBUG_PRESET
        pluginProcessor.loadDebugPreset();
#endif

#if ENABLE_MELATONINE_INSPECTOR
        inspector.setVisible(true);
        inspector.toggle(true);
#endif
    }

    ParticulesAudioProcessorEditor::~ParticulesAudioProcessorEditor() { setLookAndFeel(nullptr); }

    void ParticulesAudioProcessorEditor::paint(juce::Graphics& g) { g.fillAll(MyColours::black); }

    void ParticulesAudioProcessorEditor::resized()
    {
        int w = getWidth() / 300;
        juce::Rectangle<int> area = getLocalBounds();
        area.removeFromLeft(w);
        area.removeFromRight(w);
        mainFrame.setBounds(area);
    }
}