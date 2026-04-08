#pragma once

#include "../../../framework/GuiTypes.h"
#include "../../../framework/PluginParams.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{

    class EnvelopeButton : public juce::Component, private ValueTreeState::Listener
    {
    public:
        EnvelopeButton(ValueTreeState&, const str&, const str&);
        ~EnvelopeButton() override;

        void paint(juce::Graphics& g) override;
        void mouseDown(const juce::MouseEvent& e) override;

    private:

        void parameterChanged(const str& parameterID, float newValue) override;
        juce::Image createMenuIcon(int modeIndex, float currentSustain);

        ValueTreeState& apvts;
        str envModeId;
        str sustainId;


        juce::RangedAudioParameter* envParam;
        juce::RangedAudioParameter* sustainParam;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeButton)
    };
}