/*
  ==============================================================================

	GrainsFrame.cpp
	Created: 3 Feb 2023 11:44:06pm
	Author:  user

  ==============================================================================
*/

#include "GrainsFrame.h"
#include "../framework/PluginParams.h"
#include "../utils/MyColours.h"

GrainsFrame::GrainsFrame(ValueTreeState& apvts) : apvts{apvts}
{
    emissionSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, Param::Emission::id, emissionSlider);

    durationSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, Param::Duration::id, durationSlider);

    speedSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, Param::Speed::id, speedSlider);

    sustainRatioSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, Param::SustainRatio::id, sustainRatioSlider);

    traversalFreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, Param::TraversalFreq::id, traversalFreqSlider);

    //pitchSliderAttachment =
    //	std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    //		*apvts, PITCH_ID, pitchSlider);

    emissionSlider.setName("emissionSlider");
    emissionSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    emissionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    emissionSlider.setTextBoxIsEditable(true);
    emissionSlider.setRange(Param::Emission::min, Param::Emission::max);
    emissionSlider.setSkewFactorFromMidPoint(Param::Emission::init);
    emissionSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    emissionLabel.setText((const juce::String)Param::Emission::name, juce::dontSendNotification);
    emissionLabel.attachToComponent(&emissionSlider, false);
    emissionLabel.setJustificationType(juce::Justification::centred);

    durationSlider.setName("durationSlider");
    durationSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    durationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    durationSlider.setTextBoxIsEditable(true);
    durationSlider.setRange(Param::Duration::min, Param::Duration::max);
    durationSlider.setSkewFactorFromMidPoint(Param::Duration::init);
    durationSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    durationLabel.setText((const juce::String)Param::Duration::name, juce::dontSendNotification);
    durationLabel.attachToComponent(&durationSlider, false);
    durationLabel.setJustificationType(juce::Justification::centred);

    speedSlider.setName("speedSlider");
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    speedSlider.setTextBoxIsEditable(true);
    speedSlider.setRange(Param::Speed::min, Param::Speed::max);
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    speedLabel.setText((const juce::String)Param::Speed::name, juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(juce::Justification::centred);

    sustainRatioSlider.setName("sustainWidthSlider");
    sustainRatioSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sustainRatioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    sustainRatioSlider.setTextBoxIsEditable(true);
    sustainRatioSlider.setRange(Param::SustainRatio::min, Param::SustainRatio::max);
    sustainRatioSlider.setSkewFactorFromMidPoint(Param::SustainRatio::init);
    sustainRatioSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    sustainRatioLabel.setText((const juce::String)Param::SustainRatio::name, juce::dontSendNotification);
    sustainRatioLabel.attachToComponent(&sustainRatioSlider, false);
    sustainRatioLabel.setJustificationType(juce::Justification::centred);

    traversalFreqSlider.setName("traversalFreqSlider");
    traversalFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    traversalFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    traversalFreqSlider.setTextBoxIsEditable(true);
    traversalFreqSlider.setRange(Param::TraversalFreq::min, Param::TraversalFreq::max);
    traversalFreqSlider.setSkewFactorFromMidPoint(Param::TraversalFreq::init);
    traversalFreqSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    traversalFreqLabel.setText((const juce::String)Param::TraversalFreq::name, juce::dontSendNotification);
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

    //ComboBoxParameterAttachment(RangedAudioParameter& parameter, ComboBox& combo,
    //	UndoManager* undoManager = nullptr);

    envelopeModeList.addItemList(apvts.getParameter(Param::EnvelopeMode::id)->getAllValueStrings(), 1);

    envelopeModeList.setSelectedId(1, juce::dontSendNotification);

    envelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, Param::EnvelopeMode::id, envelopeModeList);

    traversalModeList.addItemList(apvts.getParameter(Param::TraversalMode::id)->getAllValueStrings(), 1);
    traversalModeList.setSelectedId(1, juce::dontSendNotification);

    traversalModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, Param::TraversalMode::id, traversalModeList);

    envModeLabel.setText((const juce::String)Param::EnvelopeMode::name, juce::dontSendNotification);
    envModeLabel.attachToComponent(&envelopeModeList, false);
    envModeLabel.setJustificationType(juce::Justification::centred);

    traversalModeLabel.setText((const juce::String)Param::TraversalMode::name, juce::dontSendNotification);
    traversalModeLabel.attachToComponent(&traversalModeList, false);
    traversalModeLabel.setJustificationType(juce::Justification::centred);

    /*
	envelopeList.onChange = [this] {
		this->paramsView->setEnvelopeType(envelopeList.getSelectedId());
	};

	traversalModeList.onChange = [this] {
		this->paramsView->setTraversalModeValue(traversalModeList.getSelectedId());
	};
	*/

    addAndMakeVisible(&envelopeModeList);
    addAndMakeVisible(&envModeLabel);

    addAndMakeVisible(&traversalModeList);
    addAndMakeVisible(&traversalModeLabel);

    //apvts.addParameterListener(Param::TraversalMode::id, this);
    //apvts.addParameterListener(Param::EnvelopeMode::id, this);
}

void GrainsFrame::paint(juce::Graphics& g) { g.fillAll(MyColours::black); }

void GrainsFrame::resized()
{
    float h = getHeight() / 30.f;

    juce::FlexBox mainFlexBox;
    mainFlexBox.flexDirection = juce::FlexBox::Direction::row;

    juce::FlexBox flexBox1;
    flexBox1.flexDirection = juce::FlexBox::Direction::column;
    flexBox1.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox1.alignContent = juce::FlexBox::AlignContent::center;

    flexBox1.items.add(juce::FlexItem(emissionLabel).withFlex(0.2f));
    flexBox1.items.add(juce::FlexItem(emissionSlider).withFlex(0.8f).withMargin(h));

    juce::FlexBox flexBox2;
    flexBox2.flexDirection = juce::FlexBox::Direction::column;
    flexBox2.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox2.alignContent = juce::FlexBox::AlignContent::center;

    flexBox2.items.add(juce::FlexItem(durationLabel).withFlex(0.2f));
    flexBox2.items.add(juce::FlexItem(durationSlider).withFlex(0.8f).withMargin(h));

    juce::FlexBox flexBox3;
    flexBox3.flexDirection = juce::FlexBox::Direction::column;
    flexBox3.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox3.alignContent = juce::FlexBox::AlignContent::center;

    flexBox3.items.add(juce::FlexItem(speedLabel).withFlex(0.2f));
    flexBox3.items.add(juce::FlexItem(speedSlider).withFlex(0.8f).withMargin(h));

    juce::FlexBox flexBox4;
    flexBox4.flexDirection = juce::FlexBox::Direction::column;
    flexBox4.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox4.alignContent = juce::FlexBox::AlignContent::center;

    flexBox4.items.add(juce::FlexItem(sustainRatioLabel).withFlex(0.2f));
    flexBox4.items.add(juce::FlexItem(sustainRatioSlider).withFlex(0.8f).withMargin(h));

    juce::FlexBox flexBox5;
    flexBox5.flexDirection = juce::FlexBox::Direction::column;
    flexBox5.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox5.alignContent = juce::FlexBox::AlignContent::center;

    flexBox5.items.add(juce::FlexItem(traversalFreqLabel).withFlex(0.2f));
    flexBox5.items.add(juce::FlexItem(traversalFreqSlider).withFlex(0.8f).withMargin(h));

    juce::FlexBox flexBox6;
    flexBox6.flexDirection = juce::FlexBox::Direction::column;
    flexBox6.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox6.alignContent = juce::FlexBox::AlignContent::center;

    flexBox6.items.add(juce::FlexItem(traversalModeLabel).withFlex(0.1f));
    flexBox6.items.add(juce::FlexItem(traversalModeList).withFlex(0.4f).withMaxHeight(getHeight() / 4.f).withMargin(h));
    flexBox6.items.add(juce::FlexItem(envModeLabel).withFlex(0.1f));
    flexBox6.items.add(juce::FlexItem(envelopeModeList).withFlex(0.4f).withMaxHeight(getHeight() / 4.f).withMargin(h));

    mainFlexBox.items.add(juce::FlexItem(flexBox1).withFlex(0.2f));
    mainFlexBox.items.add(juce::FlexItem(flexBox2).withFlex(0.2f));
    mainFlexBox.items.add(juce::FlexItem(flexBox3).withFlex(0.2f));
    mainFlexBox.items.add(juce::FlexItem(flexBox4).withFlex(0.2f));
    mainFlexBox.items.add(juce::FlexItem(flexBox5).withFlex(0.2f));
    mainFlexBox.items.add(juce::FlexItem(flexBox6).withFlex(0.2f));

    mainFlexBox.performLayout(getLocalBounds().toFloat());
}

//void GrainsFrame::parameterChanged(const juce::String& parameterID, float newValue)
//{
//    if(parameterID == Param::TraversalMode::id)
//    {
//        DBG("TRAVERSAL MODE parameter as value : " + (str)apvts.getParameterAsValue(Param::TraversalMode::id).toString());
//        DBG("TRAVERSAL MODE new value : " + (str)newValue);
//        return;
//    }
//    if(parameterID == Param::EnvelopeMode::id)
//    {
//        DBG("ENVELOPE MODE parameter as value : " + (str)apvts.getParameterAsValue(Param::EnvelopeMode::id).toString());
//        DBG("ENVELOPE MODE new value : " + (str)newValue);
//        return;
//    }
//}
