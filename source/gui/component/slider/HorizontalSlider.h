#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace juce
{
    class Graphics;
}

namespace particules
{
    class HorizontalSlider : public juce::Component
    {
    public:
        HorizontalSlider(float minRange, float maxRange);

        // Accès au slider pour l'attachment APVTS
        juce::Slider& getSlider() { return slider; }

        void setModulatedPosition(float pos);
        void paintOverChildren(juce::Graphics& g) override;

        float getValue() const noexcept;
        void setValue(float ) noexcept;
        std::function<void()> getOnValueChange() const noexcept;
        void setOnValueChange(std::function<void()>) noexcept;

        void resized() override;

    private:
        juce::Slider slider;
        float modulatedPos;
        //std::atomic<float> modulatedPos{0.0f};
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HorizontalSlider)
    };

}
