#include "HorizontalSliderComponent.h"

#include "../lookandfeelv2/Colours.h"

#include <juce_graphics/juce_graphics.h>

namespace particules
{
    HorizontalSliderComponent::HorizontalSliderComponent(float minRange, float maxRange) : modulatedPos{0.f}
    {
        // On configure le slider interne
        addAndMakeVisible(slider);
        slider.setSliderStyle(juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.setRange(minRange, maxRange);
    }

    void HorizontalSliderComponent::setModulatedPosition(float pos)
    {
        modulatedPos = juce::jlimit(0.0f, 1.0f, pos);
        repaint();
    }

    void HorizontalSliderComponent::paintOverChildren(juce::Graphics& g)
    {
        auto sliderBounds = slider.getBounds().toFloat();

        float xPos = sliderBounds.getX() + (sliderBounds.getWidth() * modulatedPos);
        float yPos = sliderBounds.getY() - 4.0f;

        const float indicatorSize = 8.0f;

        g.setColour(juce::Colours::white);
        juce::Rectangle<float> r(xPos - indicatorSize / 2, yPos - indicatorSize, indicatorSize, indicatorSize);

        juce::Path p;
        p.addTriangle(xPos, yPos, xPos - 4, yPos - 6, xPos + 4, yPos - 6);
        g.fillPath(p);

        g.setColour(juce::Colours::white.withAlpha(0.4f));
        g.drawLine(xPos, yPos, xPos, sliderBounds.getBottom(), 1.0f);
    }

    void HorizontalSliderComponent::resized()
    {
        auto area = getLocalBounds();
        // On laisse de l'espace en haut (25%) pour l'indicateur de modulation
        slider.setBounds(area.removeFromBottom(area.getHeight() * 0.75f));
    }

    float HorizontalSliderComponent::getValue() const noexcept { return (float)slider.getValue(); }
    void HorizontalSliderComponent::setValue(float val) noexcept { slider.setValue(val); }

    std::function<void()> HorizontalSliderComponent::getOnValueChange() const noexcept { return slider.onValueChange; }

    void HorizontalSliderComponent::setOnValueChange(std::function<void()> callback) noexcept { slider.onValueChange = callback; }
}
