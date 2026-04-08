#include "GrainsPanel.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>

#include "../../framework/bridge/EngineState.h"
#include "../../utils/UIHelpers.h"
#include "../../utils/struct/UIContext.h"
#include "../lookandfeel/MyColours.h"
#include "../lookandfeelv2/Colours.h"
#include "BinaryData.h"

namespace particules
{
    GrainsPanel::GrainsPanel(UIContext& uic)
        : engineState{uic.engineState}, emissionSlider(params::emission::name, engineState, RotaryType::primaryWithAux),
          durationSlider(params::duration::name, engineState, RotaryType::primaryWithAux),
          speedSlider(params::speed::name, engineState, RotaryType::secondaryWithAux),
          sustainRatioSlider(params::sustainRatio::name, engineState, RotaryType::secondaryWithAux),
          traversalFreqSlider(params::traversalFreq::name, engineState, RotaryType::secondaryWithAux),
          emissionSliderAttachment{emissionSlider.attachPrimaryToAPVTS(uic.apvts, params::emission::id)},
          durationSliderAttachment{durationSlider.attachPrimaryToAPVTS(uic.apvts, params::duration::id)}, linkBtn{"linkBtn"},
          speedSliderAttachment{speedSlider.attachPrimaryToAPVTS(uic.apvts, params::speed::id)},
          sustainRatioSliderAttachment{sustainRatioSlider.attachPrimaryToAPVTS(uic.apvts, params::sustainRatio::id)},
          traversalFreqSliderAttachment{traversalFreqSlider.attachPrimaryToAPVTS(uic.apvts, params::traversalFreq::id)},
          envelopeModeButton{uic.apvts, params::envelopeMode::id, params::sustainRatio::id}
    {
        // top row //

        linkInIcon = UIHelpers::loadSVG(BinaryData::link_in_svg, BinaryData::link_in_svgSize, juce::Colours::white);
        linkOffIcon = UIHelpers::loadSVG(BinaryData::link_off_svg, BinaryData::link_off_svgSize, juce::Colours::white);

        linkBtn.setIcon(linkOffIcon.get());

        linkBtn.onClick = [this]() { linkButtonClicked(); };

        emissionSlider.setOnPrimaryValueChanged([this](double emissionVal) {
            if(!engineState.getIsLinked())
                return;

            double durationVal = 1.0 / emissionVal;
            durationVal = juce::jlimit(durationSlider.getPrimaryMinimum(), durationSlider.getPrimaryMaximum(), durationVal);
            durationSlider.setPrimaryValue(durationVal, juce::dontSendNotification);
        });

        durationSlider.setOnPrimaryValueChanged([this](double durationVal) {
            if(!engineState.getIsLinked())
                return;
            double emissionVal = 1.0 / durationVal;
            emissionVal = juce::jlimit(emissionSlider.getPrimaryMinimum(), emissionSlider.getPrimaryMaximum(), emissionVal);
            emissionSlider.setPrimaryValue(emissionVal, juce::dontSendNotification);
        });

        emissionSlider.setRange(params::emission::min, params::emission::max);
        emissionSlider.setSkewFactorFromMidPoint(params::emission::skewFactor);

        durationSlider.setRange(params::duration::min, params::duration::max);
        durationSlider.setSkewFactorFromMidPoint(params::duration::skewFactor);

        addAndMakeVisible(&linkBtn);
        addAndMakeVisible(&emissionSlider);
        addAndMakeVisible(&durationSlider);

        // mid row //

        //speedSliderAttachment =
        //    std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(uic.apvts, params::speed::id, speedSlider);

        //sustainRatioSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        //    uic.apvts, params::sustainRatio::id, sustainRatioSlider);

        //traversalFreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        //    uic.apvts, params::traversalFreq::id, traversalFreqSlider);

        //pitchSliderAttachment =
        //	std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        //		*apvts, PITCH_ID, pitchSlider);

        //speedSlider.setName("speedSlider");
        speedSlider.setRange(params::speed::min, params::speed::max);

        //speedLabel.setText((const str)params::speed::name, juce::dontSendNotification);
        //speedLabel.setJustificationType(juce::Justification::centred);
        //speedLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        //speedLabel.setFont(juce::Font(13.0f));

        sustainRatioSlider.setName("sustainWidthSlider");
        sustainRatioSlider.setRange(params::sustainRatio::min, params::sustainRatio::max);
        sustainRatioSlider.setSkewFactorFromMidPoint(params::sustainRatio::skewFactor);

        //sustainRatioLabel.setText((const str)params::sustainRatio::name, juce::dontSendNotification);
        //sustainRatioLabel.setJustificationType(juce::Justification::centred);
        //sustainRatioLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        //sustainRatioLabel.setFont(juce::Font(13.0f));

        traversalFreqSlider.setName("traversalFreqSlider");
        traversalFreqSlider.setRange(params::traversalFreq::min, params::traversalFreq::max);
        traversalFreqSlider.setSkewFactorFromMidPoint((params::traversalFreq::skewFactor));

        //traversalFreqLabel.setText((const str)params::traversalFreq::name, juce::dontSendNotification);
        //traversalFreqLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        //traversalFreqLabel.setFont(juce::Font(13.0f));

        //traversalFreqLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(&speedSlider);
        addAndMakeVisible(&sustainRatioSlider);
        addAndMakeVisible(&traversalFreqSlider);

        //addAndMakeVisible(&speedLabel);
        //addAndMakeVisible(&sustainRatioLabel);
        //addAndMakeVisible(&traversalFreqLabel);

        // bottom row

        envModeLabel.setText(params::envelopeMode::name, juce::dontSendNotification);
        envModeLabel.setJustificationType(juce::Justification::centred);
        envModeLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        envModeLabel.setFont(juce::Font(13.0f));
        envModeLabel.setInterceptsMouseClicks(false, false);

        addAndMakeVisible(&envModeLabel);
        addAndMakeVisible(&envelopeModeButton);

        //addAndMakeVisible(&envModeLabel);
        //ComboBoxPlugingrainserAttachment(RangedAudioPlugingrainser& parameter, ComboBox& combo,
        //	UndoManager* undoManager = nullptr);

        //envelopeModeList.addItemList(uic.apvts.getParameter(grainsEnvelopeModeId)->getAllValueStrings(), 1);

        //envelopeModeList.setSelectedId(1, juce::dontSendNotification);

        //envelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        //uic.apvts, grainsEnvelopeModeId, envelopeModeList);

        traversalModeList.addItemList(uic.apvts.getParameter(params::traversalMode::id)->getAllValueStrings(), 1);
        traversalModeList.setSelectedId(1, juce::dontSendNotification);

        traversalModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            uic.apvts, params::traversalMode::id, traversalModeList);

        //envModeLabel.setText((const str)grainsEnvelopeModeName, juce::dontSendNotification);
        //envModeLabel.attachToComponent(&envelopeModeList, false);
        //envModeLabel.setJustificationType(juce::Justification::centred);

        traversalModeLabel.setText((const str)params::traversalMode::name, juce::dontSendNotification);
        traversalModeLabel.attachToComponent(&traversalModeList, false);
        traversalModeLabel.setJustificationType(juce::Justification::centred);

        //envelopeList.onChange = [this] { paramsView.setEnvelopeType(envelopeList.getSelectedId()); };

        //traversalModeList.onChange = [this] { paramsView.setTraversalModeValue(traversalModeList.getSelectedId()); };

        //addAndMakeVisible(&envelopeModeList);
        //addAndMakeVisible(&envModeLabel);

        //addAndMakeVisible(&traversalModeList);
        //addAndMakeVisible(&traversalModeLabel);

        //apvts.addParameterListener(grainsTraversalModeId, this);
        //apvts.addParameterListener(grainsEnvelopeModeId, this);
    }

    void GrainsPanel::linkButtonClicked() { setLinkButtonImage(); }

    void GrainsPanel::setLinkButtonImage()
    {
        if(engineState.getIsLinked())
        {
            engineState.setLink(false);
            linkBtn.setIcon(linkOffIcon.get());
        }
        else
        {
            engineState.setLink(true);
            linkBtn.setIcon(linkInIcon.get());

            double emissionVal = emissionSlider.getPrimaryValue();
            double durationVal = 1.0 / emissionVal;
            durationVal = juce::jlimit(durationSlider.getPrimaryMinimum(), durationSlider.getPrimaryMaximum(), durationVal);
            durationSlider.setPrimaryValue(durationVal, juce::dontSendNotification);
        }
    }

    void GrainsPanel::paint(juce::Graphics& g) {}

    void GrainsPanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();

        const int rowHeight = area.getHeight() / 3;
        juce::Rectangle<int> topRow = area.removeFromTop(rowHeight);
        juce::Rectangle<int> middleRow = area.removeFromTop(rowHeight);
        juce::Rectangle<int> botRow = area.removeFromTop(rowHeight);

        // top row

        const int topColWidth = juce::jmin(topRow.getWidth() / 2, rowHeight) + 5;
        const int linkBtnWidth = (area.getWidth() - topColWidth * 2) * 0.25;
        const int totalTopRowWidth = topColWidth * 2 + linkBtnWidth;

        juce::Rectangle<int> topGroup = topRow.withSizeKeepingCentre(totalTopRowWidth, topRow.getHeight());
        juce::Rectangle<int> topLeftArea = topGroup.removeFromLeft(topColWidth);
        juce::Rectangle<int> topLinkArea = topGroup.removeFromLeft(linkBtnWidth);
        juce::Rectangle<int> topRightArea = topGroup.removeFromLeft(topColWidth);

        const int btnSize = juce::jmin(topLinkArea.getWidth(), 25);

        emissionSlider.setBounds(topLeftArea);
        linkBtn.setBounds(topLinkArea.withSizeKeepingCentre(btnSize, btnSize));
        durationSlider.setBounds(topRightArea);

        // mid row

        const int midColWidth = middleRow.getWidth() / 3 ;
        speedSlider.setBounds(middleRow.removeFromLeft(midColWidth));
        sustainRatioSlider.setBounds(middleRow.removeFromLeft(midColWidth));
        traversalFreqSlider.setBounds(middleRow);

        // bottom row
        const int botColWidth = botRow.getWidth() / 2;
        envModeLabel.setBounds(botRow.removeFromLeft(botColWidth));
        envelopeModeButton.setBounds(botRow.removeFromLeft(botColWidth));
    }
}
//void GrainsPanel::parameterChanged(const str& parameterID, float newValue)
//{
//    if(parameterID == grainsTraversalMode::id)
//    {
//        DBG("TRAVERSAL MODE parameter as value : " + (str)apvts.getPlugingrainserAsValue(grainsTraversalMode::id).toString());
//        DBG("TRAVERSAL MODE new value : " + (str)newValue);
//        return;
//    }
//    if(parameterID == grainsEnvelopeMode::id)
//    {
//        DBG("ENVELOPE MODE parameter as value : " + (str)apvts.getPlugingrainserAsValue(grainsEnvelopeMode::id).toString());
//        DBG("ENVELOPE MODE new value : " + (str)newValue);
//        return;
//    }
//}

/*
void GrainsPanel::resized()
    {
        auto area = getLocalBounds();

        const float marginRatio = 0.03f;
        const float labelGapRatio = 0.12f;
        const int labelOffset = 5;

        area.removeFromRight(45);

        auto topRow = area.removeFromTop(area.getHeight() * 0.33f);
        auto middleRow = area.removeFromTop(area.getHeight() * 0.33f);
        auto bottomRow = area.removeFromTop(area.getHeight() * 0.33f);

        auto placeKnob = [&](juce::Rectangle<int> bounds, juce::Slider& slider, juce::Label& label, bool isPrimary) {
            auto cell = bounds.reduced(juce::roundToInt(bounds.getWidth() * marginRatio));

            float visualRatio = isPrimary ? 0.70f : 0.55f;
            int available = juce::jmin(cell.getWidth(), cell.getHeight());
            int visualSize = juce::jlimit(40, 160, juce::roundToInt(available * visualRatio));
            int visualRadius = visualSize / 2;

            int textMargin = isPrimary ? juce::roundToInt(visualRadius * 0.9f) : juce::roundToInt(visualRadius * 1.3f);
            int sliderBounds = visualSize + textMargin * 2;
            slider.setBounds(cell.withSizeKeepingCentre(sliderBounds, sliderBounds));
            slider.getProperties().set("visualRadius", visualRadius);

            int maxAllowed = juce::jmin(cell.getWidth(), cell.getHeight());
            if(sliderBounds > maxAllowed)
                sliderBounds = maxAllowed;

            slider.setBounds(cell.withSizeKeepingCentre(sliderBounds, sliderBounds));
            slider.getProperties().set("visualRadius", visualRadius);

            int knobBottomY = cell.getCentreY() + visualRadius;
            int labelY = knobBottomY + labelOffset;
            int labelHeight = 18;

            label.setBounds(cell.getX(), labelY, cell.getWidth(), labelHeight);
            label.setJustificationType(juce::Justification::centred);
            //label.setFont(juce::Font(13.0f));
            //label.setFont(juce::Font(juce::jmin(13.0f, visualSize * 0.16f)));
        };

        int topW = topRow.getWidth() / 2;
        placeKnob(topRow.removeFromLeft(topW), emissionSlider, emissionLabel, true);
        placeKnob(topRow, durationSlider, durationLabel, true);

        int midW = middleRow.getWidth() / 3;
        placeKnob(bottomRow.removeFromLeft(midW), speedSlider, speedLabel, false);
        placeKnob(bottomRow.removeFromLeft(midW), sustainRatioSlider, sustainRatioLabel, false);
        placeKnob(bottomRow, traversalFreqSlider, traversalFreqLabel, false);

        int botW = bottomRow.getWidth() / 3;
    }
*/