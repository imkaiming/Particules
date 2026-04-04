/**
 * This class is the graphical user interface for the plugin and is responsible
 * for displaying the controls and parameters.
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "../gui/lookandfeelv2/MainLNF.h"
//#include "../gui/lookandfeel/LightRotarySliderLNF.h"
#include "../gui/panel/MainPanel.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    class GuiAppProcessor;
    class GuiAppEditor : public juce::AudioProcessorEditor
    {
    public:
        GuiAppEditor(GuiAppProcessor&);
        ~GuiAppEditor() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        //GlobalLookAndFeel& getCustomLook() { return globalLookAndFeel; };
        const MainPanel& getMainPanel() const noexcept { return mainPanel; };

    private:
        static constexpr const int windowHeightInit = UI::windowHeightInit;
        static constexpr const int windowHeightMin = UI::windowHeightMin;
        static constexpr const int windowHeightMax = UI::windowHeightMax;

        static constexpr const int windowWidthInit = UI::windowWidthInit;
        static constexpr const int windowWidthMin = UI::windowWidthMin;
        static constexpr const int windowWidthMax = UI::windowWidthMax;

        MainPanel mainPanel;
        MainLNF lnf;
        //LightRotarySliderLNF lnf;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiAppEditor)
    };
}