#include "ButtonSliderControlGroup.h"
#include "../../lookandfeelv2/MainLNF.h"

namespace particules
{
    ButtonSliderControlGroup::ButtonSliderControlGroup(UIContext& uic, const str& n)
        : uic{uic}, name{n}, slider{uic.engineState, RotaryType::secondaryWithAux}
    {
        addAndMakeVisible(&slider);
    }

    void ButtonSliderControlGroup::paint(juce::Graphics& g)
    {
        g.setColour(colours::perleBlanc);
        g.drawRoundedRectangle(getLocalBounds().toFloat(), 10.f, 1.0f);

        const int labelWidth = getWidth() / 8.f;
        juce::Rectangle<float> labelArea = getLocalBounds().removeFromLeft(labelWidth).toFloat();

        //g.setColour(colours::perleBlanc);
        g.setFont(dynamic_cast<MainLNF*>(&getLookAndFeel())->getFont().withExtraKerningFactor(0.2f));

        g.saveState();
        g.addTransform(juce::AffineTransform::rotation(-halfPi, labelArea.getCentreX(), labelArea.getCentreY()));
        juce::Rectangle<float> verticalTextRect(
            labelArea.getCentreX() - (getHeight() / 2.0f), labelArea.getCentreY() - labelWidth / 2, (float)getHeight(), 25.0f);
        g.drawText(name, verticalTextRect, juce::Justification::centred);
        g.restoreState();
    }

    void ButtonSliderControlGroup::resized()
    {
        juce::Rectangle<int> bounds = getLocalBounds();

        const int labelWidth = getWidth() / 8.f;
        bounds.removeFromLeft(labelWidth);
        bounds.removeFromRight(labelWidth);
        //bounds.reduce(5, 5);
        juce::Rectangle<int> topArea = bounds.removeFromTop(bounds.getHeight() * 0.5f);

        const int buttonPadding = getWidth() / 16.f;

        getButton().setBounds(topArea.reduced(buttonPadding));
        slider.setBounds(bounds);
    }

    void ButtonSliderControlGroup::setupSlider(const str& paramId, float min, float max, float skewFactor)
    {
        slider.setRange(min, max);
        slider.setSkewFactorFromMidPoint(skewFactor);
        attachment = slider.attachPrimaryToAPVTS(uic.apvts, paramId);
    }
}