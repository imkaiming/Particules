#include "SelectionOverlayComponent.h"


namespace particules
{

    SelectionOverlayComponent::SelectionOverlayComponent() : start{0.f}, width{1.f} { /*setSelection(0.f);*/ }

    void SelectionOverlayComponent::setPosition(float newValue)
    {
        start = newValue;
        repaint();
    }

    void SelectionOverlayComponent::setSelection(float newValue)
    {
        width = newValue;
        repaint();
    }

    void SelectionOverlayComponent::paint(juce::Graphics& g)
    {
        g.setColour(MyColours::red);

        if(start + width > getWidth())
        {
            g.fillRect(start + 1.f, (float)getHeight() - 2.f, getWidth() - start, 4.f);
        }
        else
        {
            g.fillRect(start + 1.f, (float)getHeight() - 2.f, width, 4.f);
        }
    }

}