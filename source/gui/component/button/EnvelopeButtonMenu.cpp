#include "EnvelopeButtonMenu.h"
#include "../../../utils/enum/EnvelopeMode.h"
#include "../../../utils/math/EnvelopeMath.h"
#include "../../lookandfeelv2/Colours.h"

namespace particules
{
    // change the init value with the apvts get state restore in the plugin processor
    EnvelopeButtonMenu::EnvelopeButtonMenu(ValueTreeState& state, const str& envModeParamId, const str& sustainParamId)
        : apvts(state), envelopeModeId(envModeParamId), envelopeRatioId(sustainParamId), cachedMode{params::envelopeMode::init},
          cachedEnvelopeRatio{params::envelopeRatio::init}
    {
        envelopeModeParam = apvts.getParameter(envelopeModeId);
        envelopeRatioParam = apvts.getParameter(envelopeRatioId);

        apvts.addParameterListener(envelopeModeId, this);
        apvts.addParameterListener(envelopeRatioId, this);
    }

    EnvelopeButtonMenu::~EnvelopeButtonMenu()
    {
        apvts.removeParameterListener(envelopeModeId, this);
        apvts.removeParameterListener(envelopeRatioId, this);
    }

    void EnvelopeButtonMenu::parameterChanged(const str& parameterID, float newValue)
    {
        if(parameterID == envelopeModeId)
        {
            cachedMode = static_cast<EnvelopeMode>(juce::jlimit(
                0, (int)EnvelopeMode::Count - 1, (int)(newValue * (envelopeModeParam->getAllValueStrings().size() - 1))));
        }
        else if(parameterID == envelopeRatioId)
        {
            cachedEnvelopeRatio = envelopeRatioParam->convertFrom0to1(newValue);
        }

        juce::MessageManager::callAsync([this]() { repaint(); });
    }

    juce::Path EnvelopeButtonMenu::createCurvePath(juce::Rectangle<float> bounds)
    {
        EnvelopeMode mode = getCurrentMode();
        const float sustain = getCurrentSustain();

        return buildPathFromFunction(
            bounds, [mode, sustain](float phase) { return particules::gui::evaluateEnvelopeWithPlateau(mode, phase, sustain); });
    }

    void EnvelopeButtonMenu::showPopupMenu()
    {
        juce::PopupMenu menu;
        juce::StringArray options = envelopeModeParam->getAllValueStrings();
        for(int i = 0; i < options.size(); ++i)
        {
            menu.addItem(i + 1, "" /* options[i]*/, true, false, createMenuIcon(i));
        }

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this), [this, options](int result) {
            if(result > 0)
            {
                int selectedIndex = result - 1;
                float newValue = static_cast<float>(selectedIndex) / (options.size() - 1);
                envelopeModeParam->setValueNotifyingHost(newValue);
                cachedMode = static_cast<EnvelopeMode>(selectedIndex);
                repaint();
            }
        });
    }

    juce::Image EnvelopeButtonMenu::createMenuIcon(int itemIndex)
    {
        const int w = 200;
        const int h = 40;
        juce::Image img(juce::Image::ARGB, w, h, true);
        juce::Graphics g(img);

        g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);

        auto mode = static_cast<EnvelopeMode>(itemIndex);
        const float forcedSustain = 0.0f; 

        juce::Path p = buildPathFromFunction(
            {5.0f, 5.0f, w - 10.0f, h - 10.0f},
            [mode, forcedSustain](
                float phase) { return particules::gui::evaluateEnvelopeWithPlateau(mode, phase, forcedSustain); },
            w);

        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(2.5f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));

        return img;
    }
}

/*
    EnvelopeMode EnvelopeButtonMenu::getCurrentMode() const
    {
        int index = getCurrentModeIndex();
        return static_cast<EnvelopeMode>(index);
    }

    int EnvelopeButtonMenu::getCurrentModeIndex() const
    {
        return static_cast<int>(envelopeModeParam->getValue() * (envelopeModeParam->getAllValueStrings().size() - 1));
    }

    float EnvelopeButtonMenu::getCurrentSustain() const { return sustainParam->convertFrom0to1(sustainParam->getValue()); }
    */

/*
    void EnvelopeButtonMenu::paint(juce::Graphics& g)
    {

        bool isHover = isMouseOver();

        juce::Rectangle<float> bounds = getLocalBounds().toFloat();

        // Fond du bouton (sombre)
        g.setColour(isHover ? juce::Colour(0xff1e1e1e).brighter(0.05) : juce::Colour(0xff1e1e1e));
        g.fillRoundedRectangle(bounds, 5.0f);

        // Bordure (allumée si survolée)
        g.setColour(isMouseOver() ? juce::Colours::white.withAlpha(0.5f) : coloursv2::midGrey);
        g.drawRoundedRectangle(bounds, 5.0f, 1.0f);

        EnvelopeMode mode = static_cast<EnvelopeMode>((int)envelopeModeParam->getValue() * (envelopeModeParam->getAllValueStrings().size() - 1));
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

        g.setColour(coloursv2::cyan);
        g.strokePath(p, juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));
}
    */