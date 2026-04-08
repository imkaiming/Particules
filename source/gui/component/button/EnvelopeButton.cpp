#include "EnvelopeButton.h"
#include "../../../utils/enum/EnvelopeMode.h"
#include "../../../utils/math/EnvelopeMath.h"
#include "../../lookandfeelv2/Colours.h"

namespace particules
{
    EnvelopeButton::EnvelopeButton(ValueTreeState& state, const str& envModeParamId, const str& sustainParamId)
        : apvts(state), envModeId(envModeParamId), sustainId(sustainParamId)
    {
        envParam = apvts.getParameter(envModeId);
        sustainParam = apvts.getParameter(sustainId);

        apvts.addParameterListener(envModeId, this);
        apvts.addParameterListener(sustainId, this);

        //setMouseCursor(juce::MouseCursor::PointingHandCursor); // do a highlight on mouse hover instead
    }

    EnvelopeButton::~EnvelopeButton()
    {
        apvts.removeParameterListener(envModeId, this);
        apvts.removeParameterListener(sustainId, this);
    }

    void EnvelopeButton::parameterChanged(const str& /*parameterID*/, float /*newValue*/)
    {
        // Thread safe : indique à JUCE de redessiner sur le thread principal
        juce::MessageManager::callAsync([this]() { repaint(); });
    }

    void EnvelopeButton::paint(juce::Graphics& g)
    {
        auto bounds = getLocalBounds().toFloat();

        // Fond du bouton (sombre)
        g.setColour(juce::Colour(0xff1e1e1e));
        g.fillRoundedRectangle(bounds, 5.0f);

        // Bordure (allumée si survolée)
        g.setColour(isMouseOver() ? juce::Colours::white.withAlpha(0.5f) : coloursv2::midGrey);
        g.drawRoundedRectangle(bounds, 5.0f, 1.0f);

        EnvelopeMode mode = static_cast<EnvelopeMode>((int)envParam->getValue() * (envParam->getAllValueStrings().size() - 1));
        float sustain = sustainParam->convertFrom0to1(sustainParam->getValue());

        juce::Path p;
        const float width = bounds.getWidth();
        const float height = bounds.getHeight();
        const float padding = 4.0f;
        const float drawWidth = width - (padding * 2.0f);
        const float drawHeight = height - (padding * 2.0f);

        // Dessin dynamique de l'enveloppe avec plateau
        for(int i = 0; i <= drawWidth; ++i)
        {
            float phase = static_cast<float>(i) / drawWidth;
            float val = particules::gui::evaluateEnvelopeWithPlateau(mode, phase, sustain);

            float x = padding + static_cast<float>(i);
            float y = padding + drawHeight * (1.0f - val);

            if(i == 0)
                p.startNewSubPath(x, y);
            else
                p.lineTo(x, y);
        }

        g.setColour(juce::Colours::cyan);
        g.strokePath(p, juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));
    }

    void EnvelopeButton::mouseDown(const juce::MouseEvent& /*e*/)
    {
        juce::PopupMenu menu;
        auto options = envParam->getAllValueStrings();

        int currentIndex = (int)(envParam->getValue() * (options.size() - 1));
        float currentSustain = sustainParam->convertFrom0to1(sustainParam->getValue());

        for(int i = 0; i < options.size(); ++i)
        {
            auto icon = createMenuIcon(i, currentSustain);
            menu.addItem(i + 1, options[i], true, i == currentIndex, icon);
        }

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this), [this, options](int result) {
            if(result != 0)
            {
                int selectedIndex = result - 1;
                float normalizedValue = (float)selectedIndex / (float)(options.size() - 1);
                envParam->setValueNotifyingHost(normalizedValue);
            }
        });
    }

    juce::Image EnvelopeButton::createMenuIcon(int modeIndex, float currentSustain)
    {
        juce::Image img(juce::Image::ARGB, 60, 20, true);
        juce::Graphics g(img);

        juce::Path p;
        auto mode = static_cast<EnvelopeMode>(modeIndex);

        for(int i = 0; i <= 60; ++i)
        {
            float phase = static_cast<float>(i) / 60.0f;
            float val = particules::gui::evaluateEnvelopeWithPlateau(mode, phase, currentSustain);
            float x = static_cast<float>(i);
            float y = 18.0f * (1.0f - val) + 1.0f;

            if(i == 0)
                p.startNewSubPath(x, y);
            else
                p.lineTo(x, y);
        }

        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(1.5f));
        return img;
    }

}