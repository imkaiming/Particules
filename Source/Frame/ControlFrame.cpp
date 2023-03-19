/*
  ==============================================================================

	ControlFrame.cpp
	Created: 2 Mar 2023 7:45:17pm
	Author:  user

  ==============================================================================
*/

#include "ControlFrame.h"

ControlFrame::ControlFrame(juce::AudioProcessorValueTreeState* apvts, StateParameters* stateParams) :
	apvts(apvts), stateParams(stateParams)
{
	mixSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, MIX_ID, mixSlider);

	gainSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, GAIN_ID, gainSlider);

	densitySliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, DENSITY_ID, densitySlider);

	durationSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, DURATION_ID, durationSlider);

	speedSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, SPEED_ID, speedSlider);

	//pitchSliderAttachment =
	//	std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
	//		*apvts, PITCH_ID, pitchSlider);

	mixSlider.setName("mixSlider");
	mixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	mixSlider.setTextBoxIsEditable(true);
	mixSlider.setRange(0.0, 100.0);
	mixSlider.setTextValueSuffix(" %");
	mixSlider.addListener(this);


	mixLabel.setText((const juce::String)MIX_NAME, juce::dontSendNotification);
	mixLabel.attachToComponent(&mixSlider, false);


	gainSlider.setName("gainSlider");
	gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	mixSlider.setTextBoxIsEditable(true);
	gainSlider.setRange(GAIN_MIN, GAIN_MAX);
	gainSlider.setSkewFactorFromMidPoint(-12.0);
	gainSlider.setTextValueSuffix(" dB");
	gainSlider.addListener(this);


	gainLabel.setText((const juce::String)GAIN_NAME, juce::dontSendNotification);
	gainLabel.attachToComponent(&gainSlider, false);


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


	speedSlider.setName("speedSlider");
	speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	speedSlider.setTextBoxIsEditable(true);
	speedSlider.setRange(SPEED_MIN, SPEED_MAX);
	speedSlider.addListener(this);

	speedLabel.setText((const juce::String)SPEED_NAME, juce::dontSendNotification);
	speedLabel.attachToComponent(&speedSlider, false);



	addAndMakeVisible(&mixSlider);
	addAndMakeVisible(&gainSlider);
	addAndMakeVisible(&densitySlider);
	addAndMakeVisible(&durationSlider);
	addAndMakeVisible(&speedSlider);


	addAndMakeVisible(&mixLabel);
	addAndMakeVisible(&gainLabel);
	addAndMakeVisible(&densityLabel);
	addAndMakeVisible(&durationLabel);
	addAndMakeVisible(&speedLabel);

	//ComboBoxParameterAttachment(RangedAudioParameter& parameter, ComboBox& combo,
	//	UndoManager* undoManager = nullptr);


	envelopeList.addItem(ENVTYPE_1, 1);
	envelopeList.addItem(ENVTYPE_2, 2);
	envelopeList.addItem(ENVTYPE_3, 3);
	envelopeList.setSelectedId(1, juce::dontSendNotification); // default value set to Gaussian

	envelopeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
		*apvts, ENVTYPE_ID, envelopeList);

	//envelopeAttachment = std::make_unique<juce::ComboBoxParameterAttachment>(
	//	apvts->getParameter(ENVTYPE_ID), envelopeList);

	envelopeList.onChange = [this] {
		this->stateParams->setEnvelopeType(envelopeList.getSelectedId());
	};

	addAndMakeVisible(&envelopeList);


}

ControlFrame::~ControlFrame()
{
	mixSlider.removeListener(this);
	gainSlider.removeListener(this);
	densitySlider.removeListener(this);
	durationSlider.removeListener(this);
	speedSlider.removeListener(this);
}

void ControlFrame::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::coral);
}

void ControlFrame::resized()
{
	juce::FlexBox layout;
	layout.flexDirection = juce::FlexBox::Direction::column;
	layout.justifyContent = juce::FlexBox::JustifyContent::center;

	juce::FlexBox fbLabel{

	};

	juce::FlexBox fbSlider{
		juce::FlexBox::Direction::row,
		juce::FlexBox::Wrap::noWrap,
		juce::FlexBox::AlignContent::stretch,
		juce::FlexBox::AlignItems::stretch,
		juce::FlexBox::JustifyContent::center
	};


	juce::FlexBox fbCombo{

	};

	fbCombo.items.add(juce::FlexItem(envelopeList).withFlex(1).withMargin(5));

	fbSlider.items.add(juce::FlexItem(mixSlider).withFlex(1).withMargin(5));
	fbSlider.items.add(juce::FlexItem(densitySlider).withFlex(1).withMargin(5));
	fbSlider.items.add(juce::FlexItem(durationSlider).withFlex(1).withMargin(5));
	fbSlider.items.add(juce::FlexItem(speedSlider).withFlex(1).withMargin(5));
	fbSlider.items.add(juce::FlexItem(gainSlider).withFlex(1).withMargin(5));

	fbLabel.items.add(juce::FlexItem(mixLabel).withFlex(1).withMargin(5));
	fbLabel.items.add(juce::FlexItem(densityLabel).withFlex(1).withMargin(5));
	fbLabel.items.add(juce::FlexItem(durationLabel).withFlex(1).withMargin(5));
	fbLabel.items.add(juce::FlexItem(speedLabel).withFlex(1).withMargin(5));
	fbLabel.items.add(juce::FlexItem(gainLabel).withFlex(1).withMargin(5));

	layout.items.add(juce::FlexItem(fbLabel).withFlex(1));
	layout.items.add(juce::FlexItem(fbSlider).withFlex(1));
	layout.items.add(juce::FlexItem(fbCombo).withFlex(1));

	layout.performLayout(getLocalBounds().toFloat());
}

//double numToDb(double num) {
//	return (pow(10, num / 20));
//}


// Note that calling getRawParameterValue() within a 
// AudioProcessorValueTreeState::Listener::ParameterChanged()
// WILL NOT RETURN A UP-TO-DATE VALUE !!
void ControlFrame::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &mixSlider) {
		apvts->getRawParameterValue(MIX_ID)->store(static_cast<float>(mixSlider.getValue()));
		stateParams->setMix(static_cast<float>(mixSlider.getValue()));
		//this->apvts->mix = mixSlider.getValue();
	}
	if (slider == &gainSlider) {
		apvts->getRawParameterValue(GAIN_ID)->store(
			static_cast<float>((gainSlider.getValue())));
		stateParams->setGain(
			static_cast<float>(gainSlider.getValue()));

	}
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

