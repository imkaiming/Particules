#include "GrainsPanel.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>

#include "../../utils/struct/UIContext.h"
#include "../lookandfeel/MyColours.h"
#include "../lookandfeelv2/Colours.h"
#include "BinaryData.h"

namespace particules
{
    GrainsPanel::GrainsPanel(UIContext& uic)
        : /*{uic.apvts},  paramsView{uic.paramsView}, */ engineState{uic.engineState},
          emissionSlider(grainsEmissionName, engineState), durationSlider(grainsDurationName, engineState),
          emissionSliderAttachment{emissionSlider.attachPrimaryToAPVTS(uic.apvts, grainsEmissionId)},
          durationSliderAttachment{durationSlider.attachPrimaryToAPVTS(uic.apvts, grainsDurationId)},
          linkBtn{(const str) "linkBtn", juce::DrawableButton::ButtonStyle::ImageFitted}
    {
        setLinkButtonImage();

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

        emissionSlider.setRange(grainsEmissionMin, grainsEmissionMax);
        emissionSlider.setSkewFactorFromMidPoint(grainsEmissionSkewFactor);

        durationSlider.setRange(grainsDurationMin, grainsDurationMax);
        durationSlider.setSkewFactorFromMidPoint(grainsDurationSkewFactor);

        speedSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(uic.apvts, grainsSpeedId, speedSlider);

        sustainRatioSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            uic.apvts, grainsSustainRatioId, sustainRatioSlider);

        traversalFreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            uic.apvts, grainsTraversalFreqId, traversalFreqSlider);

        //pitchSliderAttachment =
        //	std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        //		*apvts, PITCH_ID, pitchSlider);

        speedSlider.setName("speedSlider");
        speedSlider.setRange(grainsSpeedMin, grainsSpeedMax);

        speedLabel.setText((const str)grainsSpeedName, juce::dontSendNotification);
        speedLabel.setJustificationType(juce::Justification::centred);
        speedLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        speedLabel.setFont(juce::Font(13.0f));

        sustainRatioSlider.setName("sustainWidthSlider");
        sustainRatioSlider.setRange(grainsSustainRatioMin, grainsSustainRatioMax);
        sustainRatioSlider.setSkewFactorFromMidPoint(grainsSustainRatioSkewFactor);

        sustainRatioLabel.setText((const str)grainsSustainRatioName, juce::dontSendNotification);
        sustainRatioLabel.setJustificationType(juce::Justification::centred);
        sustainRatioLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        sustainRatioLabel.setFont(juce::Font(13.0f));

        traversalFreqSlider.setName("traversalFreqSlider");
        traversalFreqSlider.setRange(grainsTraversalFreqMin, grainsTraversalFreqMax);
        traversalFreqSlider.setSkewFactorFromMidPoint(grainsTraversalFreqSkewFactor);

        traversalFreqLabel.setText((const str)grainsTraversalFreqName, juce::dontSendNotification);
        traversalFreqLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        traversalFreqLabel.setFont(juce::Font(13.0f));

        traversalFreqLabel.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(&linkBtn);
        addAndMakeVisible(&emissionSlider);
        addAndMakeVisible(&durationSlider);
        addAndMakeVisible(&speedSlider);
        addAndMakeVisible(&sustainRatioSlider);
        addAndMakeVisible(&traversalFreqSlider);

        addAndMakeVisible(&speedLabel);
        addAndMakeVisible(&sustainRatioLabel);
        addAndMakeVisible(&traversalFreqLabel);

        //ComboBoxPlugingrainserAttachment(RangedAudioPlugingrainser& parameter, ComboBox& combo,
        //	UndoManager* undoManager = nullptr);

        envelopeModeList.addItemList(uic.apvts.getParameter(grainsEnvelopeModeId)->getAllValueStrings(), 1);

        envelopeModeList.setSelectedId(1, juce::dontSendNotification);

        envelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            uic.apvts, grainsEnvelopeModeId, envelopeModeList);

        traversalModeList.addItemList(uic.apvts.getParameter(grainsTraversalModeId)->getAllValueStrings(), 1);
        traversalModeList.setSelectedId(1, juce::dontSendNotification);

        traversalModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            uic.apvts, grainsTraversalModeId, traversalModeList);

        envModeLabel.setText((const str)grainsEnvelopeModeName, juce::dontSendNotification);
        envModeLabel.attachToComponent(&envelopeModeList, false);
        envModeLabel.setJustificationType(juce::Justification::centred);

        traversalModeLabel.setText((const str)grainsTraversalModeName, juce::dontSendNotification);
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
            linkBtn.setImages(juce::Drawable::createFromImageData(BinaryData::link_off_svg, BinaryData::link_off_svgSize).get(),
                juce::Drawable::createFromImageData(BinaryData::link_off_svg, BinaryData::link_off_svgSize).get(), nullptr,
                nullptr, nullptr, nullptr, nullptr, nullptr);
        }
        else
        {
            engineState.setLink(true);
            linkBtn.setImages(juce::Drawable::createFromImageData(BinaryData::link_in_svg, BinaryData::link_in_svgSize).get(),
                juce::Drawable::createFromImageData(BinaryData::link_in_svg, BinaryData::link_in_svgSize).get(), nullptr, nullptr,
                nullptr, nullptr, nullptr, nullptr);

            double emissionVal = emissionSlider.getPrimaryValue();
            double durationVal = 1.0 / emissionVal;
            durationVal = juce::jlimit(durationSlider.getPrimaryMinimum(), durationSlider.getPrimaryMaximum(), durationVal);
            durationSlider.setPrimaryValue(durationVal, juce::dontSendNotification);
        }
    }

    void GrainsPanel::paint(juce::Graphics& g) {}

    void GrainsPanel::resized()
    {
        auto area = getLocalBounds();

        const int rowHeight = area.getHeight() / 3;
        auto topRow = area.removeFromTop(rowHeight);
        auto middleRow = area.removeFromTop(rowHeight);

        const int topColWidth = juce::jmin(topRow.getWidth() / 2, rowHeight) + 5;
        const int linkBtnWidth = (area.getWidth() - topColWidth * 2) * 0.25;
        const int totalWidth = topColWidth * 2 + linkBtnWidth;
        auto group = topRow.withSizeKeepingCentre(totalWidth, topRow.getHeight());

        auto left = group.removeFromLeft(topColWidth);
        auto link = group.removeFromLeft(linkBtnWidth);
        auto right = group.removeFromLeft(topColWidth);

        emissionSlider.setBounds(left);
        linkBtn.setBounds(link);
        durationSlider.setBounds(right);

        const int midColWidth = middleRow.getWidth() / 3;
        speedSlider.setBounds(middleRow.removeFromLeft(midColWidth));
        sustainRatioSlider.setBounds(middleRow.removeFromLeft(midColWidth));
        traversalFreqSlider.setBounds(middleRow);

        // Bottom row: available in 'area' variable for future components
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