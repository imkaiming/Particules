/**
 * This class is the graphical user interface for the plugin and is responsible
 * for displaying the controls and parameters.
*/

#pragma once

#include "PluginProcessor.h"
#include "gui/panel/MainPanel.h"
#include "gui/lookandfeel/GlobalLookAndFeel.h"
#include "gui/lookandfeelv2/MainLNF.h"

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

        GlobalLookAndFeel& getCustomLook() { return globalLookAndFeel; };
        const MainPanel& getMainPanel() const noexcept { return mainPanel; };

    private:
        static constexpr const int windowHeightInit = gui::windowHeightInit;
        static constexpr const int windowHeightMin = gui::windowHeightMin;
        static constexpr const int windowHeightMax = gui::windowHeightMax;

        static constexpr const int windowWidthInit = gui::windowWidthInit;
        static constexpr const int windowWidthMin = gui::windowWidthMin;
        static constexpr const int windowWidthMax = gui::windowWidthMax;

        MainLNF lnf;
        ParticulesAudioProcessor& pluginProcessor;
        MainPanel mainPanel;
        GlobalLookAndFeel globalLookAndFeel;
#if ENABLE_MELATONINE_INSPECTOR
        melatonin::Inspector inspector{*this};
#endif

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessorEditor)
    };
}