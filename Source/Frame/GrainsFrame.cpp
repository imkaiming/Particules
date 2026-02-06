/*
  ==============================================================================

	GrainsFrame.cpp
	Created: 3 Feb 2023 11:44:06pm
	Author:  user

  ==============================================================================
*/

#include "GrainsFrame.h"
#include "../Utils/StateParameters.h"


GrainsFrame::GrainsFrame(juce::AudioProcessorValueTreeState* apvts, StateParameters* stateParams) :
	apvts(apvts), stateParams(stateParams)
{
	densitySliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, Param::Density::id, densitySlider);

	durationSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, Param::Duration::id, durationSlider);

	speedSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, Param::Speed::id, speedSlider);

	envWidthSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, Param::EnvelopeWidth::id, envWidthSlider);

	traversalTimeSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, Param::TraversalTime::id, traversalTimeSlider);

	//pitchSliderAttachment =
	//	std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
	//		*apvts, PITCH_ID, pitchSlider);

	densitySlider.setName("densitySlider");
	densitySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	densitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	densitySlider.setTextBoxIsEditable(true);
	densitySlider.setRange(Param::Density::min, Param::Density::max);
	densitySlider.setSkewFactorFromMidPoint(Param::Density::init);
	densitySlider.setTextValueSuffix(" g");
	//densitySlider.addListener(this);
	densitySlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

	densityLabel.setText((const juce::String)Param::Density::name, juce::dontSendNotification);
	densityLabel.attachToComponent(&densitySlider, false);
	densityLabel.setJustificationType(juce::Justification::centred);

	durationSlider.setName("durationSlider");
	durationSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	durationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	durationSlider.setTextBoxIsEditable(true);
	durationSlider.setRange(Param::Duration::min, Param::Duration::max);
	durationSlider.setSkewFactorFromMidPoint(Param::Duration::init);
	durationSlider.setTextValueSuffix(" s");
	//durationSlider.addListener(this);
	durationSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

	durationLabel.setText((const juce::String)Param::Duration::name, juce::dontSendNotification);
	durationLabel.attachToComponent(&durationSlider, false);
	durationLabel.setJustificationType(juce::Justification::centred);

	speedSlider.setName("speedSlider");
	speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	speedSlider.setTextBoxIsEditable(true);
	speedSlider.setRange(Param::Speed::min, Param::Speed::max);
	//speedSlider.addListener(this);
	speedSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

	speedLabel.setText((const juce::String)Param::Speed::name, juce::dontSendNotification);
	speedLabel.attachToComponent(&speedSlider, false);
	speedLabel.setJustificationType(juce::Justification::centred);


	envWidthSlider.setName("envWidthSlider");
	envWidthSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	envWidthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	envWidthSlider.setTextBoxIsEditable(true);
	envWidthSlider.setRange(Param::EnvelopeWidth::min, Param::EnvelopeWidth::max);
	envWidthSlider.setSkewFactorFromMidPoint(Param::EnvelopeWidth::init);
	//envWidthSlider.setTextValueSuffix(" g");
	//envWidthSlider.addListener(this);
	envWidthSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

	envWidthLabel.setText((const juce::String)Param::EnvelopeWidth::name, juce::dontSendNotification);
	envWidthLabel.attachToComponent(&envWidthSlider, false);
	envWidthLabel.setJustificationType(juce::Justification::centred);


	traversalTimeSlider.setName("traversalTimeSlider");
	traversalTimeSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	traversalTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	traversalTimeSlider.setTextBoxIsEditable(true);
	traversalTimeSlider.setRange(Param::TraversalTime::min, Param::TraversalTime::max);
	traversalTimeSlider.setSkewFactorFromMidPoint(Param::TraversalTime::init);
	traversalTimeSlider.setTextValueSuffix(" s");
	//traversalTimeSlider.addListener(this);
	traversalTimeSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

	traversalTimeLabel.setText((const juce::String)Param::TraversalTime::name, juce::dontSendNotification);
	traversalTimeLabel.attachToComponent(&traversalTimeSlider, false);
	traversalTimeLabel.setJustificationType(juce::Justification::centred);


	addAndMakeVisible(&densitySlider);
	addAndMakeVisible(&durationSlider);
	addAndMakeVisible(&speedSlider);
	addAndMakeVisible(&envWidthSlider);
	addAndMakeVisible(&traversalTimeSlider);

	addAndMakeVisible(&densityLabel);
	addAndMakeVisible(&durationLabel);
	addAndMakeVisible(&speedLabel);
	addAndMakeVisible(&envWidthLabel);
	addAndMakeVisible(&traversalTimeLabel);

	//ComboBoxParameterAttachment(RangedAudioParameter& parameter, ComboBox& combo,
	//	UndoManager* undoManager = nullptr);


	envelopeList.addItem(Param::EnvelopeType::ENVTYPE_1, 1);
	envelopeList.addItem(Param::EnvelopeType::ENVTYPE_2, 2);
	envelopeList.addItem(Param::EnvelopeType::ENVTYPE_3, 3);
	envelopeList.addItem(Param::EnvelopeType::ENVTYPE_4, 4);
	envelopeList.addItem(Param::EnvelopeType::ENVTYPE_5, 5);
	envelopeList.addItem(Param::EnvelopeType::ENVTYPE_6, 6);
	envelopeList.addItem(Param::EnvelopeType::ENVTYPE_7, 7);
	envelopeList.setSelectedId(1, juce::dontSendNotification); // default value set to Hann window

	traversalModeList.addItem(Param::TraversalMode::TraversalMode_1, 1);
	traversalModeList.addItem(Param::TraversalMode::TraversalMode_2, 2);
	traversalModeList.addItem(Param::TraversalMode::TraversalMode_3, 3);
	traversalModeList.addItem(Param::TraversalMode::TraversalMode_4, 4);
	traversalModeList.addItem(Param::TraversalMode::TraversalMode_5, 5);
	traversalModeList.setSelectedId(1, juce::dontSendNotification); // default value set to sinus mode

	envelopeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
		*apvts, Param::EnvelopeType::id, envelopeList);
	//envelopeAttachment = std::make_unique<juce::ComboBoxParameterAttachment>(
	//	apvts->getParameter(ENVTYPE_ID), envelopeList);

	traversalModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
		*apvts, Param::TraversalMode::id, traversalModeList);

	envTypeLabel.setText((const juce::String)Param::EnvelopeType::name, juce::dontSendNotification);
	envTypeLabel.attachToComponent(&envelopeList, false);
	envTypeLabel.setJustificationType(juce::Justification::centred);

	traversalModeLabel.setText((const juce::String)Param::TraversalMode::name, juce::dontSendNotification);
	traversalModeLabel.attachToComponent(&traversalModeList, false);
	traversalModeLabel.setJustificationType(juce::Justification::centred);

	/*
	envelopeList.onChange = [this] {
		this->stateParams->setEnvelopeType(envelopeList.getSelectedId());
	};

	traversalModeList.onChange = [this] {
		this->stateParams->setTraversalModeValue(traversalModeList.getSelectedId());
	};
	*/

	addAndMakeVisible(&envelopeList);
	addAndMakeVisible(&envTypeLabel);

	addAndMakeVisible(&traversalModeList);
	addAndMakeVisible(&traversalModeLabel);
}
GrainsFrame::~GrainsFrame()
{
	//densitySlider.removeListener(this);
	//durationSlider.removeListener(this);
	//speedSlider.removeListener(this);
	//envWidthSlider.removeListener(this);
	//traversalTimeSlider.removeListener(this);

	apvts = nullptr;
	stateParams = nullptr;
}

void GrainsFrame::paint(juce::Graphics& g)
{
	g.fillAll(MyColours::black);

}

void GrainsFrame::resized()
{
	float h = getHeight() / 30.f;

	juce::FlexBox mainFlexBox;
	mainFlexBox.flexDirection = juce::FlexBox::Direction::row;

	juce::FlexBox flexBox1;
	flexBox1.flexDirection = juce::FlexBox::Direction::column;
	flexBox1.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox1.alignContent = juce::FlexBox::AlignContent::center;

	flexBox1.items.add(juce::FlexItem(densityLabel).withFlex(0.2));
	flexBox1.items.add(juce::FlexItem(densitySlider).withFlex(0.8).withMargin(h));

	juce::FlexBox flexBox2;
	flexBox2.flexDirection = juce::FlexBox::Direction::column;
	flexBox2.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox2.alignContent = juce::FlexBox::AlignContent::center;

	flexBox2.items.add(juce::FlexItem(durationLabel).withFlex(0.2));
	flexBox2.items.add(juce::FlexItem(durationSlider).withFlex(0.8).withMargin(h));

	juce::FlexBox flexBox3;
	flexBox3.flexDirection = juce::FlexBox::Direction::column;
	flexBox3.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox3.alignContent = juce::FlexBox::AlignContent::center;

	flexBox3.items.add(juce::FlexItem(speedLabel).withFlex(0.2));
	flexBox3.items.add(juce::FlexItem(speedSlider).withFlex(0.8).withMargin(h));

	juce::FlexBox flexBox4;
	flexBox4.flexDirection = juce::FlexBox::Direction::column;
	flexBox4.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox4.alignContent = juce::FlexBox::AlignContent::center;

	flexBox4.items.add(juce::FlexItem(envWidthLabel).withFlex(0.2));
	flexBox4.items.add(juce::FlexItem(envWidthSlider).withFlex(0.8).withMargin(h));

	juce::FlexBox flexBox5;
	flexBox5.flexDirection = juce::FlexBox::Direction::column;
	flexBox5.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox5.alignContent = juce::FlexBox::AlignContent::center;

	flexBox5.items.add(juce::FlexItem(traversalTimeLabel).withFlex(0.2));
	flexBox5.items.add(juce::FlexItem(traversalTimeSlider).withFlex(0.8).withMargin(h));

	juce::FlexBox flexBox6;
	flexBox6.flexDirection = juce::FlexBox::Direction::column;
	flexBox6.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox6.alignContent = juce::FlexBox::AlignContent::center;

	flexBox6.items.add(juce::FlexItem(traversalModeLabel).withFlex(0.1));
	flexBox6.items.add(juce::FlexItem(traversalModeList).withFlex(0.4).withMaxHeight(getHeight() / 4).withMargin(h));
	flexBox6.items.add(juce::FlexItem(envTypeLabel).withFlex(0.1));
	flexBox6.items.add(juce::FlexItem(envelopeList).withFlex(0.4).withMaxHeight(getHeight() / 4).withMargin(h));


	mainFlexBox.items.add(juce::FlexItem(flexBox1).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox2).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox3).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox4).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox5).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox6).withFlex(0.20));

	mainFlexBox.performLayout(getLocalBounds().toFloat());
}

// Note that calling getRawParameterValue() within a 
// AudioProcessorValueTreeState::Listener::ParameterChanged()
// WILL NOT RETURN A UP-TO-DATE VALUE !!
/*
void GrainsFrame::sliderValueChanged(juce::Slider* slider)
{

	if (slider == &densitySlider) {
		apvts->getRawParameterValue(Param::Density::id)->store(static_cast<float>(densitySlider.getValue()));
		stateParams->setDensity(static_cast<float>(densitySlider.getValue()));
	}
	if (slider == &durationSlider) {
		apvts->getRawParameterValue(Param::Duration::id)->store(static_cast<float>(durationSlider.getValue()));
		stateParams->setDuration(static_cast<float>(durationSlider.getValue()));
	}
	if (slider == &speedSlider) {
		apvts->getRawParameterValue(Param::Speed::id)->store(static_cast<float>(speedSlider.getValue()));
		stateParams->setSpeed(static_cast<float>(speedSlider.getValue()));
	}
	if (slider == &envWidthSlider) {
		apvts->getRawParameterValue(Param::EnvelopeWidth::id)->store(static_cast<float>(envWidthSlider.getValue()));
		stateParams->setEnvWidth(static_cast<float>(envWidthSlider.getValue()));
	}
	if (slider == &traversalTimeSlider) {
		apvts->getRawParameterValue(Param::TraversalTime::id)->store(static_cast<float>(traversalTimeSlider.getValue()));
		stateParams->setTraversalTimeValue(static_cast<float>(traversalTimeSlider.getValue()));
	}
}*/
