/**
 * This class is the graphical user interface for the plugin and is responsible
 * for displaying the controls and parameters.
*/

#pragma once

#include "PluginProcessor.h"
#include "gui/panel/MainPanel.h"
#include "utils/lookandfeel/GlobalLookAndFeel.h"

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
        static constexpr const int windowHeightInit = UI::windowHeightInit;
        static constexpr const int windowHeightMin = UI::windowHeightMin;
        static constexpr const int windowHeightMax = UI::windowHeightMax;

        static constexpr const int windowWidthInit = UI::windowWidthInit;
        static constexpr const int windowWidthMin = UI::windowWidthMin;
        static constexpr const int windowWidthMax = UI::windowWidthMax;

        ParticulesAudioProcessor& pluginProcessor;
        MainPanel mainPanel;
        GlobalLookAndFeel globalLookAndFeel;

#if ENABLE_MELATONINE_INSPECTOR
        melatonin::Inspector inspector{*this};
#endif

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParticulesAudioProcessorEditor)
    };
}