#include "GuiAppEditor.h"
#include "GuiAppProcessor.h"
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>


namespace particules
{
    GuiAppEditor::GuiAppEditor(GuiAppProcessor& p) : AudioProcessorEditor(&p), mainPanel(p.getUIContext()), lnf()
    {
        juce::LookAndFeel::setDefaultLookAndFeel(&lnf);
        //setLookAndFeel(&lnf);

        setResizable(true, true);
        setResizeLimits(gui::windowWidthMin, gui::windowHeightMin, gui::windowWidthMax, gui::windowHeightMax);

        const float ratio = 1.5f; // ratio 3:2
        getConstrainer()->setFixedAspectRatio(ratio);

        setSize(gui::windowWidthInit, gui::windowHeightInit);


        addAndMakeVisible(&mainPanel);
    }

    GuiAppEditor::~GuiAppEditor() { juce::LookAndFeel::setDefaultLookAndFeel(nullptr); }

    //void GuiAppEditor::paint(juce::Graphics& /*g*/) {}

    void GuiAppEditor::resized() { mainPanel.setBounds(getLocalBounds()); }
}