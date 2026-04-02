#include "GrainsPanel.h"
#include "../../framework/bridge/ParameterView.h"
#include "../../utils/MyColours.h"
#include "../../utils/struct/UIContext.h"

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
        emissionSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        emissionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
        emissionSlider.setTextBoxIsEditable(true);
        emissionSlider.setRange(grainsEmissionMin, grainsEmissionMax);
        emissionSlider.setSkewFactorFromMidPoint(grainsEmissionSkewFactor);
        emissionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        emissionLabel.setText((const str)grainsEmissionName, juce::dontSendNotification);
        emissionLabel.attachToComponent(&emissionSlider, false);
        emissionLabel.setJustificationType(juce::Justification::centred);

        durationSlider.setName("durationSlider");
        durationSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        durationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
        durationSlider.setTextBoxIsEditable(true);
        durationSlider.setRange(grainsDurationMin, grainsDurationMax);
        durationSlider.setSkewFactorFromMidPoint(grainsDurationSkewFactor);
        durationSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        durationLabel.setText((const str)grainsDurationName, juce::dontSendNotification);
        durationLabel.attachToComponent(&durationSlider, false);
        durationLabel.setJustificationType(juce::Justification::centred);

        speedSlider.setName("speedSlider");
        speedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
        speedSlider.setTextBoxIsEditable(true);
        speedSlider.setRange(grainsSpeedMin, grainsSpeedMax);
        speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        speedLabel.setText((const str)grainsSpeedName, juce::dontSendNotification);
        speedLabel.attachToComponent(&speedSlider, false);
        speedLabel.setJustificationType(juce::Justification::centred);

        sustainRatioSlider.setName("sustainWidthSlider");
        sustainRatioSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        sustainRatioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
        sustainRatioSlider.setTextBoxIsEditable(true);
        sustainRatioSlider.setRange(grainsSustainRatioMin, grainsSustainRatioMax);
        sustainRatioSlider.setSkewFactorFromMidPoint(grainsSustainRatioSkewFactor);
        sustainRatioSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        sustainRatioLabel.setText((const str)grainsSustainRatioName, juce::dontSendNotification);
        sustainRatioLabel.attachToComponent(&sustainRatioSlider, false);
        sustainRatioLabel.setJustificationType(juce::Justification::centred);

        traversalFreqSlider.setName("traversalFreqSlider");
        traversalFreqSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        traversalFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
        traversalFreqSlider.setTextBoxIsEditable(true);
        traversalFreqSlider.setRange(grainsTraversalFreqMin, grainsTraversalFreqMax);
        traversalFreqSlider.setSkewFactorFromMidPoint(grainsTraversalFreqSkewFactor);
        traversalFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        traversalFreqLabel.setText((const str)grainsTraversalFreqName, juce::dontSendNotification);
        traversalFreqLabel.attachToComponent(&traversalFreqSlider, false);
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
        juce::Rectangle<int> area = getLocalBounds().reduced(10);

        const int labelHeight = 20;
        const int numKnobs = 5;

        const int knobSize = juce::jmin(area.getHeight() - labelHeight, area.getWidth() / numKnobs);

        const int totalHeight = labelHeight + knobSize;
        const int yOffset = (area.getHeight() - totalHeight) / 2;

        juce::Rectangle<int> row = area.withY(area.getY() + yOffset).withHeight(totalHeight);

        const int spacing = (row.getWidth() - knobSize * numKnobs) / (numKnobs + 1);

        int x = row.getX() + spacing;

        auto place = [&](juce::Label& label, juce::Slider& slider) {
            label.setBounds(x, row.getY(), knobSize, labelHeight);
            slider.setBounds(x, row.getY() + labelHeight, knobSize, knobSize);
            x += knobSize + spacing;
        };

        place(emissionLabel, emissionSlider);
        place(durationLabel, durationSlider);
        place(speedLabel, speedSlider);
        place(sustainRatioLabel, sustainRatioSlider);
        place(traversalFreqLabel, traversalFreqSlider);
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
