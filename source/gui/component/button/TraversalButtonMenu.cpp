#include "TraversalButtonMenu.h"
#include "../../../utils/math/EnvelopeMath.h"

namespace particules
{
    TraversalButtonMenu::TraversalButtonMenu(ValueTreeState& apvts, const str& traversalModeId, const str& traversalFreqId)
        : apvts{apvts}, traversalModeId{traversalModeId}, traversalFreqId{traversalFreqId},
          cachedMode{params::traversalMode::init}, cachedFreq{params::traversalFreq::init}
    {
        traversalModeParam = apvts.getParameter(traversalModeId);
        traversalFreqParam = apvts.getParameter(traversalFreqId);

        cachedMode = static_cast<TraversalMode>(traversalModeParam->getValue());
        cachedFreq = static_cast<float>(traversalFreqParam->getValue());

        apvts.addParameterListener(traversalModeId, this);
        apvts.addParameterListener(traversalFreqId, this);
    }

    TraversalButtonMenu::~TraversalButtonMenu()
    {
        apvts.removeParameterListener(traversalModeId, this);
        apvts.removeParameterListener(traversalFreqId, this);
    }

    juce::Path TraversalButtonMenu::createCurvePath(juce::Rectangle<float> bounds)
    {
        TraversalMode mode = getCurrentMode();
        const float range = getCurrentFrequency();

        return buildPathFromFunction(
            bounds, [mode, range](float phase) { return particules::gui::evaluateTraversalCurve(mode, phase, range); });
    }

    void TraversalButtonMenu::showPopupMenu()
    {
        juce::PopupMenu menu;

        juce::StringArray options = traversalModeParam->getAllValueStrings();

        for(int i = 0; i < options.size(); ++i)
        {
            menu.addItem(i + 1, "" /* options[i]*/, true, false, createMenuIcon(i));
        }

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this), [this, options](int result) {
            if(result > 0)
            {
                int selectedIndex = result - 1;
                float newValue = static_cast<float>(selectedIndex) / (options.size() - 1);
                traversalModeParam->setValueNotifyingHost(newValue);
                cachedMode = static_cast<TraversalMode>(selectedIndex);
                repaint();
            }
        });
    }

    juce::Image TraversalButtonMenu::createMenuIcon(int itemIndex)
    {
        const int imgWidth = 120;
        const int imgHeight = 30;
        juce::Image img(juce::Image::ARGB, imgWidth, imgHeight, true);
        juce::Graphics g(img);

        const TraversalMode mode = static_cast<TraversalMode>(itemIndex);

        const float pureFreq = 1.0f;

        juce::Path p = buildPathFromFunction(
            {0, 0, (float)imgWidth, (float)imgHeight},
            [mode, pureFreq](float phase) { return particules::gui::evaluateTraversalCurve(mode, phase, pureFreq); }, imgWidth);

        g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));

        return img;
    }

    void TraversalButtonMenu::parameterChanged(const str& parameterID, float newValue)
    {
        if(parameterID == traversalModeId)
        {
            cachedMode = static_cast<TraversalMode>(juce::jlimit(
                0, (int)TraversalMode::Count - 1, (int)(newValue * (traversalModeParam->getAllValueStrings().size() - 1))));
        }
        else if(parameterID == traversalFreqId)
        {
            cachedFreq = traversalFreqParam->convertFrom0to1(newValue);
        }

        juce::MessageManager::callAsync([this]() { repaint(); });
    }

}