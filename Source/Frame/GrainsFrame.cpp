/*
  ==============================================================================

	GrainsFrame.cpp
	Created: 3 Feb 2023 11:44:06pm
	Author:  user

  ==============================================================================
*/

#include "GrainsFrame.h"

GrainsFrame::GrainsFrame(ValueTreeState* apvts, StateParameters* stateParams) :
	apvts(apvts), stateParams(stateParams)
{
	densitySliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, DENSITY_ID, densitySlider);

	durationSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, DURATION_ID, durationSlider);

	speedSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, SPEED_ID, speedSlider);

	envWidthSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, ENVWIDTH_ID, envWidthSlider);

	//pitchSliderAttachment =
	//	std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
	//		*apvts, PITCH_ID, pitchSlider);

	densitySlider.setName("densitySlider");
	densitySlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	densitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	densitySlider.setTextBoxIsEditable(true);
	densitySlider.setRange(DENSITY_MIN, DENSITY_MAX);
	densitySlider.setSkewFactorFromMidPoint(DENSITY_DEFAULT);
	densitySlider.setTextValueSuffix(" g");
	densitySlider.addListener(this);


	densityLabel.setText((const juce::String)DENSITY_NAME, juce::dontSendNotification);
	densityLabel.attachToComponent(&densitySlider, false);
	densityLabel.setJustificationType(juce::Justification::centred);

	durationSlider.setName("durationSlider");
	durationSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	durationSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	durationSlider.setTextBoxIsEditable(true);
	durationSlider.setRange(DURATION_MIN, DURATION_MAX);
	durationSlider.setSkewFactorFromMidPoint(DURATION_DEFAULT);
	durationSlider.setTextValueSuffix(" s");
	durationSlider.addListener(this);


	durationLabel.setText((const juce::String)DURATION_NAME, juce::dontSendNotification);
	durationLabel.attachToComponent(&durationSlider, false);
	durationLabel.setJustificationType(juce::Justification::centred);

	speedSlider.setName("speedSlider");
	speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	speedSlider.setTextBoxIsEditable(true);
	speedSlider.setRange(SPEED_MIN, SPEED_MAX);
	speedSlider.addListener(this);


	speedLabel.setText((const juce::String)SPEED_NAME, juce::dontSendNotification);
	speedLabel.attachToComponent(&speedSlider, false);
	speedLabel.setJustificationType(juce::Justification::centred);


	envWidthSlider.setName("envWidthSlider");
	envWidthSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	envWidthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	envWidthSlider.setTextBoxIsEditable(true);
	envWidthSlider.setRange(ENVWIDTH_MIN, ENVWIDTH_MAX);
	envWidthSlider.setSkewFactorFromMidPoint(ENVWIDTH_DEFAULT);
	//envWidthSlider.setTextValueSuffix(" g");
	envWidthSlider.addListener(this);


	envWidthLabel.setText((const juce::String)ENVWIDTH_NAME, juce::dontSendNotification);
	envWidthLabel.attachToComponent(&envWidthSlider, false);
	envWidthLabel.setJustificationType(juce::Justification::centred);


	addAndMakeVisible(&densitySlider);
	addAndMakeVisible(&durationSlider);
	addAndMakeVisible(&speedSlider);
	addAndMakeVisible(&envWidthSlider);

	addAndMakeVisible(&densityLabel);
	addAndMakeVisible(&durationLabel);
	addAndMakeVisible(&speedLabel);
	addAndMakeVisible(&envWidthLabel);

	//ComboBoxParameterAttachment(RangedAudioParameter& parameter, ComboBox& combo,
	//	UndoManager* undoManager = nullptr);


	envelopeList.addItem(ENVTYPE_1, 1);
	envelopeList.addItem(ENVTYPE_2, 2);
	envelopeList.addItem(ENVTYPE_3, 3);
	envelopeList.setSelectedId(1, juce::dontSendNotification); // default value set to Gaussian

	envelopeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
		*apvts, ENVTYPE_ID, envelopeList);

	//envelopeAttachment = std::make_unique<juce::ComboBoxParameterAttachment>(
	//	apvts->getParameter(ENVTYPE_ID), envelopeList);

	envelopeList.onChange = [this] {
		this->stateParams->setEnvelopeType(envelopeList.getSelectedId());
	};

	addAndMakeVisible(&envelopeList);
}
GrainsFrame::~GrainsFrame()
{
	densitySlider.removeListener(this);
	durationSlider.removeListener(this);
	speedSlider.removeListener(this);
	envWidthSlider.removeListener(this);

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

	flexBox1.items.add(juce::FlexItem(densitySlider).withFlex(0.8).withMargin(h));
	flexBox1.items.add(juce::FlexItem(densityLabel).withFlex(0.2));

	juce::FlexBox flexBox2;
	flexBox2.flexDirection = juce::FlexBox::Direction::column;
	flexBox2.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox2.alignContent = juce::FlexBox::AlignContent::center;

	flexBox2.items.add(juce::FlexItem(durationSlider).withFlex(0.8).withMargin(h));
	flexBox2.items.add(juce::FlexItem(durationLabel).withFlex(0.2));

	juce::FlexBox flexBox3;
	flexBox3.flexDirection = juce::FlexBox::Direction::column;
	flexBox3.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox3.alignContent = juce::FlexBox::AlignContent::center;

	flexBox3.items.add(juce::FlexItem(speedSlider).withFlex(0.8).withMargin(h));
	flexBox3.items.add(juce::FlexItem(speedLabel).withFlex(0.2));

	juce::FlexBox flexBox4;
	flexBox4.flexDirection = juce::FlexBox::Direction::column;
	flexBox4.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox4.alignContent = juce::FlexBox::AlignContent::center;

	flexBox4.items.add(juce::FlexItem(envelopeList).withFlex(0.8).withMaxHeight(getHeight() / 2).withMargin(h));
	//flexBox4.items.add(juce::FlexItem(speedLabel).withFlex(0.2));

	juce::FlexBox flexBox5;
	flexBox5.flexDirection = juce::FlexBox::Direction::column;
	flexBox5.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox5.alignContent = juce::FlexBox::AlignContent::center;

	flexBox5.items.add(juce::FlexItem(envWidthSlider).withFlex(0.8).withMargin(h));
	flexBox5.items.add(juce::FlexItem(envWidthLabel).withFlex(0.2));

	mainFlexBox.items.add(juce::FlexItem(flexBox1).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox2).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox3).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox5).withFlex(0.20));
	mainFlexBox.items.add(juce::FlexItem(flexBox4).withFlex(0.20));

	mainFlexBox.performLayout(getLocalBounds().toFloat());
}

// Note that calling getRawParameterValue() within a 
// AudioProcessorValueTreeState::Listener::ParameterChanged()
// WILL NOT RETURN A UP-TO-DATE VALUE !!
void GrainsFrame::sliderValueChanged(juce::Slider* slider)
{

	if (slider == &densitySlider) {
		apvts->getRawParameterValue(DENSITY_ID)->store(static_cast<float>(densitySlider.getValue()));
		stateParams->setDensity(static_cast<float>(densitySlider.getValue()));
	}
	if (slider == &durationSlider) {
		apvts->getRawParameterValue(DURATION_ID)->store(static_cast<float>(durationSlider.getValue()));
		stateParams->setDuration(static_cast<float>(durationSlider.getValue()));
	}
	if (slider == &speedSlider) {
		apvts->getRawParameterValue(SPEED_ID)->store(static_cast<float>(speedSlider.getValue()));
		stateParams->setSpeed(static_cast<float>(speedSlider.getValue()));
	}

}
