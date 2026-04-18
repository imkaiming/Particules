#include "TabPanel.h"

#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/PluginParams.h"
#include "gui/lookandfeelv2/Colours.h"
#include "utils/struct/UIContext.h"

namespace particules
{
    TabPanel::TabPanel(UIContext& uic) : uic{uic}, grainsPanel(uic)
    {
        const int tabRadioGroup = 101; // unique ID for the radio group

        auto setupTabButton = [this, tabRadioGroup](juce::TextButton& btn, int index) {
            btn.setClickingTogglesState(true);
            btn.setRadioGroupId(tabRadioGroup);
            btn.onClick = [this, index] { showTab(index); };
            addAndMakeVisible(btn);
        };

        grainTabButton.getProperties().set("isTab", true);
        modTabButton.getProperties().set("isTab", true);
        regionTabButton.getProperties().set("isTab", true);

        setupTabButton(grainTabButton, 0);
        setupTabButton(modTabButton, 1);
        setupTabButton(regionTabButton, 2);

        addAndMakeVisible(grainsPanel);
        //addChildComponent(modPanelPlaceholder);
        //addChildComponent(regionPanelPlaceholder);

        // Default State
        grainTabButton.setToggleState(true, juce::dontSendNotification);
        showTab(0);
    }

    void TabPanel::paint(juce::Graphics& g) {}

    void TabPanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();

        const int tabsHeigth = area.getHeight() / 10.f;
        juce::Rectangle<int> tabArea = area.removeFromTop(tabsHeigth);

        const int buttonWidth = tabArea.getWidth() / 3;
        grainTabButton.setBounds(tabArea.removeFromLeft(buttonWidth));
        modTabButton.setBounds(tabArea.removeFromLeft(buttonWidth));
        regionTabButton.setBounds(tabArea);

        grainsPanel.setBounds(area);
        // modPanel.setBounds(area);
        // regionPanel.setBounds(area);
    }

    void TabPanel::showTab(int tabIndex)
    {
        grainsPanel.setVisible(tabIndex == 0);

        // modPanel.setVisible(tabIndex == 1);
        // regionPanel.setVisible(tabIndex == 2);
    }
}