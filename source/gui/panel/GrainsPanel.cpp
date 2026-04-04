#include "GrainsPanel.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>

#include "../../utils/struct/UIContext.h"
#include "../lookandfeel/MyColours.h"
#include "../lookandfeelv2/Colours.h"

namespace particules
{
    GrainsPanel::GrainsPanel(UIContext& uic) : apvts{uic.apvts}, paramsView{uic.paramsView}
    {
        emissionSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, grainsEmissionId, emissionSlider);

        durationSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, grainsDurationId, durationSlider);

        speedSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, grainsSpeedId, speedSlider);

        sustainRatioSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, grainsSustainRatioId, sustainRatioSlider);

        traversalFreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts, grainsTraversalFreqId, traversalFreqSlider);

        //pitchSliderAttachment =
        //	std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        //		*apvts, PITCH_ID, pitchSlider);

        emissionSlider.setName("emissionSlider");
        emissionSlider.setTextBoxIsEditable(true);
        emissionSlider.setRange(grainsEmissionMin, grainsEmissionMax);
        emissionSlider.setSkewFactorFromMidPoint(grainsEmissionSkewFactor);
        //emissionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow);

        emissionLabel.setText((const str)grainsEmissionName, juce::dontSendNotification);
        emissionLabel.setJustificationType(juce::Justification::centred);
        emissionLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        emissionLabel.setFont(juce::Font(13.0f));

        durationSlider.setName("durationSlider");
        durationSlider.setTextBoxIsEditable(true);
        durationSlider.setRange(grainsDurationMin, grainsDurationMax);
        durationSlider.setSkewFactorFromMidPoint(grainsDurationSkewFactor);

        durationLabel.setText((const str)grainsDurationName, juce::dontSendNotification);
        durationLabel.setJustificationType(juce::Justification::centred);
        durationLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        durationLabel.setFont(juce::Font(13.0f));

        speedSlider.setName("speedSlider");
        speedSlider.setTextBoxIsEditable(true);
        speedSlider.setRange(grainsSpeedMin, grainsSpeedMax);

        speedLabel.setText((const str)grainsSpeedName, juce::dontSendNotification);
        speedLabel.setJustificationType(juce::Justification::centred);
        speedLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        speedLabel.setFont(juce::Font(13.0f));


        sustainRatioSlider.setName("sustainWidthSlider");
        sustainRatioSlider.setTextBoxIsEditable(true);
        sustainRatioSlider.setRange(grainsSustainRatioMin, grainsSustainRatioMax);
        sustainRatioSlider.setSkewFactorFromMidPoint(grainsSustainRatioSkewFactor);

        sustainRatioLabel.setText((const str)grainsSustainRatioName, juce::dontSendNotification);
        sustainRatioLabel.setJustificationType(juce::Justification::centred);
        sustainRatioLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        sustainRatioLabel.setFont(juce::Font(13.0f));

        traversalFreqSlider.setName("traversalFreqSlider");
        traversalFreqSlider.setTextBoxIsEditable(true);
        traversalFreqSlider.setRange(grainsTraversalFreqMin, grainsTraversalFreqMax);
        traversalFreqSlider.setSkewFactorFromMidPoint(grainsTraversalFreqSkewFactor);

        traversalFreqLabel.setText((const str)grainsTraversalFreqName, juce::dontSendNotification);
        traversalFreqLabel.setColour(juce::Label::textColourId, colours::perleBlanc);
        traversalFreqLabel.setFont(juce::Font(13.0f));
        
        traversalFreqLabel.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(&emissionSlider);
        addAndMakeVisible(&durationSlider);
        addAndMakeVisible(&speedSlider);
        addAndMakeVisible(&sustainRatioSlider);
        addAndMakeVisible(&traversalFreqSlider);

        addAndMakeVisible(&emissionLabel);
        addAndMakeVisible(&durationLabel);
        addAndMakeVisible(&speedLabel);
        addAndMakeVisible(&sustainRatioLabel);
        addAndMakeVisible(&traversalFreqLabel);

        //ComboBoxPlugingrainserAttachment(RangedAudioPlugingrainser& parameter, ComboBox& combo,
        //	UndoManager* undoManager = nullptr);

        envelopeModeList.addItemList(apvts.getParameter(grainsEnvelopeModeId)->getAllValueStrings(), 1);

        envelopeModeList.setSelectedId(1, juce::dontSendNotification);

        envelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, grainsEnvelopeModeId, envelopeModeList);

        traversalModeList.addItemList(apvts.getParameter(grainsTraversalModeId)->getAllValueStrings(), 1);
        traversalModeList.setSelectedId(1, juce::dontSendNotification);

        traversalModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
            apvts, grainsTraversalModeId, traversalModeList);

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
        /*
        */
    }

    void GrainsPanel::paint(juce::Graphics& g) {}

    void GrainsPanel::resized()
    {
        auto area = getLocalBounds();

        const float marginRatio = 0.03f;
        const float labelGapRatio = 0.12f;
        const int labelOffset = 5;

        area.removeFromRight(45);

        auto topRow = area.removeFromTop(area.getHeight() * 0.6f);
        auto bottomRow = area;

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

        int botW = bottomRow.getWidth() / 3;
        placeKnob(bottomRow.removeFromLeft(botW), speedSlider, speedLabel, false);
        placeKnob(bottomRow.removeFromLeft(botW), sustainRatioSlider, sustainRatioLabel, false);
        placeKnob(bottomRow, traversalFreqSlider, traversalFreqLabel, false);
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
