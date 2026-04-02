#include "GuiAppEditor.h"

namespace particules
{
    GuiAppEditor::GuiAppEditor(GuiAppProcessor& p) : AudioProcessorEditor(&p), mainPanel(p.getUIContext()), globalLookAndFeel()
    {
        setLookAndFeel(&lnf);

        setResizable(true, true);
        setResizeLimits(windowWidthMin, windowHeightMin, windowWidthMax, windowHeightMax);

        const float ratio = 1.5f; // ratio 3:2
        getConstrainer()->setFixedAspectRatio(ratio);

        setSize(windowWidthInit, windowHeightInit);

        addAndMakeVisible(&mainPanel);
    }

    GuiAppEditor::~GuiAppEditor() { setLookAndFeel(nullptr); }

    void GuiAppEditor::paint(juce::Graphics& g) {}

    void GuiAppEditor::resized() { mainPanel.setBounds(getLocalBounds()); }
}