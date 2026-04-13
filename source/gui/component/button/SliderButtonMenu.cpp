#include "SliderButtonMenu.h"

namespace particules
{
    SliderButtonMenu::SliderButtonMenu(const str& name)
    {
        setRepaintsOnMouseActivity(true);

        nameLabel.setText(name, juce::dontSendNotification);
        nameLabel.setJustificationType(juce::Justification::centred);
        nameLabel.setColour(juce::Label::textColourId, coloursv2::perleBlanc);
        addAndMakeVisible(nameLabel);
    }

    void SliderButtonMenu::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();
        nameLabel.setBounds(area.removeFromBottom(20));

        buttonBounds = area.toFloat().reduced(5.f);
        //buttonBounds = getLocalBounds().toFloat().reduced(5.f);
    }

    void SliderButtonMenu::mouseDown(const juce::MouseEvent& e)
    {
        isDragging = false;
        mouseDownPosition = e.getPosition();
        valueOnMouseDown = getDragValue(); // On capture la valeur initiale
    }

    void SliderButtonMenu::mouseDrag(const juce::MouseEvent& e)
    {
        // Seuil de 3 pixels pour éviter de déclencher le drag sur un clic tremblant
        if(!isDragging && e.getDistanceFromDragStart() > 3)
        {
            isDragging = true;
        }

        if(isDragging)
        {
            // Mouvement vertical standard pour un slider
            float dragDistance = static_cast<float>(mouseDownPosition.y - e.y);
            float sensitivity = 200.0f; // Nombre de pixels pour faire 0.0 -> 1.0

            float newValue = juce::jlimit(0.0f, 1.0f, valueOnMouseDown + (dragDistance / sensitivity));
            setDragValue(newValue);
        }
    }

    void SliderButtonMenu::mouseUp(const juce::MouseEvent& e)
    {
        if(!isDragging)
        {
            showPopupMenu();
        }
        isDragging = false;
    }

    void SliderButtonMenu::paint(juce::Graphics& g)
    {
        const bool isHover = isMouseOver();
        const float w = juce::jmin(buttonBounds.getWidth(), buttonBounds.getHeight());
        auto drawArea = buttonBounds.reduced(w / 4.f);

        // 1. Fond du bouton
        g.setColour(isHover ? coloursv2::deepBlack.brighter(0.02f) : coloursv2::deepBlack);
        g.fillRoundedRectangle(buttonBounds, 6.0f);

        // 2. RETOUR VISUEL (Arc ou Ligne) pour l'affordance du Slider
        float currentValue = getDragValue();
        juce::Path valueTrack;
        // Exemple : un arc en bas du bouton
        valueTrack.addRoundedRectangle(
            buttonBounds.getX(), buttonBounds.getBottom() - 4.0f, buttonBounds.getWidth() * currentValue, 4.0f, 2.0f);
        g.setColour(curveColour.withAlpha(0.5f));
        g.fillPath(valueTrack);

        // 3. Dessin de la courbe (Menu)
        juce::Path curve = createCurvePath(drawArea);
        g.setColour(curveColour);
        g.strokePath(curve, juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));
    }

    juce::Path SliderButtonMenu::buildPathFromFunction(
        juce::Rectangle<float> bounds, std::function<float(float)> evalFunc, int numPoints)
    {
        juce::Path p;
        const float padding = 4.0f;
        const float drawWidth = bounds.getWidth() - (padding * 2.0f);
        const float drawHeight = bounds.getHeight() - (padding * 2.0f);

        for(int i = 0; i <= numPoints; ++i)
        {
            float phase = static_cast<float>(i) / numPoints;
            float val = evalFunc(phase);

            float x = bounds.getX() + padding + (phase * drawWidth);
            float y = bounds.getY() + padding + drawHeight * (1.0f - val);

            if(i == 0)
                p.startNewSubPath(x, y);
            else
                p.lineTo(x, y);
        }

        return p;
    }
}