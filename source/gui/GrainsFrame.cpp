#include "GrainsFrame.h"
#include "../utils/PluginParams.h"
#include "../utils/MyColours.h"

GrainsFrame::GrainsFrame(ValueTreeState& apvts) : apvts{apvts}
{
    emissionSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, PluginParams::Emission::id, emissionSlider);

    durationSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, PluginParams::Duration::id, durationSlider);

    speedSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, PluginParams::Speed::id, speedSlider);

    sustainRatioSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, PluginParams::SustainRatio::id, sustainRatioSlider);

    traversalFreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, PluginParams::TraversalFreq::id, traversalFreqSlider);

    //pitchSliderAttachment =
    //	std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    //		*apvts, PITCH_ID, pitchSlider);

    emissionSlider.setName("emissionSlider");
    emissionSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    emissionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    emissionSlider.setTextBoxIsEditable(true);
    emissionSlider.setRange(PluginParams::Emission::min, PluginParams::Emission::max);
    emissionSlider.setSkewFactorFromMidPoint(PluginParams::Emission::init);
    emissionSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    emissionLabel.setText((const juce::String)PluginParams::Emission::name, juce::dontSendNotification);
    emissionLabel.attachToComponent(&emissionSlider, false);
    emissionLabel.setJustificationType(juce::Justification::centred);

    durationSlider.setName("durationSlider");
    durationSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    durationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    durationSlider.setTextBoxIsEditable(true);
    durationSlider.setRange(PluginParams::Duration::min, PluginParams::Duration::max);
    durationSlider.setSkewFactorFromMidPoint(PluginParams::Duration::init);
    durationSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    durationLabel.setText((const juce::String)PluginParams::Duration::name, juce::dontSendNotification);
    durationLabel.attachToComponent(&durationSlider, false);
    durationLabel.setJustificationType(juce::Justification::centred);

    speedSlider.setName("speedSlider");
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    speedSlider.setTextBoxIsEditable(true);
    speedSlider.setRange(PluginParams::Speed::min, PluginParams::Speed::max);
    speedSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    speedLabel.setText((const juce::String)PluginParams::Speed::name, juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(juce::Justification::centred);

    sustainRatioSlider.setName("sustainWidthSlider");
    sustainRatioSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    sustainRatioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    sustainRatioSlider.setTextBoxIsEditable(true);
    sustainRatioSlider.setRange(PluginParams::SustainRatio::min, PluginParams::SustainRatio::max);
    sustainRatioSlider.setSkewFactorFromMidPoint(PluginParams::SustainRatio::init);
    sustainRatioSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    sustainRatioLabel.setText((const juce::String)PluginParams::SustainRatio::name, juce::dontSendNotification);
    sustainRatioLabel.attachToComponent(&sustainRatioSlider, false);
    sustainRatioLabel.setJustificationType(juce::Justification::centred);

    traversalFreqSlider.setName("traversalFreqSlider");
    traversalFreqSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    traversalFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    traversalFreqSlider.setTextBoxIsEditable(true);
    traversalFreqSlider.setRange(PluginParams::TraversalFreq::min, PluginParams::TraversalFreq::max);
    traversalFreqSlider.setSkewFactorFromMidPoint(PluginParams::TraversalFreq::init);
    traversalFreqSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    traversalFreqLabel.setText((const juce::String)PluginParams::TraversalFreq::name, juce::dontSendNotification);
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

    //ComboBoxPluginParamseterAttachment(RangedAudioPluginParamseter& parameter, ComboBox& combo,
    //	UndoManager* undoManager = nullptr);

    envelopeModeList.addItemList(apvts.getParameter(PluginParams::EnvelopeMode::id)->getAllValueStrings(), 1);

    envelopeModeList.setSelectedId(1, juce::dontSendNotification);

    envelopeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, PluginParams::EnvelopeMode::id, envelopeModeList);

    traversalModeList.addItemList(apvts.getParameter(PluginParams::TraversalMode::id)->getAllValueStrings(), 1);
    traversalModeList.setSelectedId(1, juce::dontSendNotification);

    traversalModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, PluginParams::TraversalMode::id, traversalModeList);

    envModeLabel.setText((const juce::String)PluginParams::EnvelopeMode::name, juce::dontSendNotification);
    envModeLabel.attachToComponent(&envelopeModeList, false);
    envModeLabel.setJustificationType(juce::Justification::centred);

    traversalModeLabel.setText((const juce::String)PluginParams::TraversalMode::name, juce::dontSendNotification);
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

    //apvts.addPluginParamseterListener(PluginParams::TraversalMode::id, this);
    //apvts.addPluginParamseterListener(PluginParams::EnvelopeMode::id, this);
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
//    if(parameterID == PluginParams::TraversalMode::id)
//    {
//        DBG("TRAVERSAL MODE parameter as value : " + (str)apvts.getPluginParamseterAsValue(PluginParams::TraversalMode::id).toString());
//        DBG("TRAVERSAL MODE new value : " + (str)newValue);
//        return;
//    }
//    if(parameterID == PluginParams::EnvelopeMode::id)
//    {
//        DBG("ENVELOPE MODE parameter as value : " + (str)apvts.getPluginParamseterAsValue(PluginParams::EnvelopeMode::id).toString());
//        DBG("ENVELOPE MODE new value : " + (str)newValue);
//        return;
//    }
//}
