/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"

AudioFileFrame::AudioFileFrame(ValueTreeState* apvts, StateParameters* stateParams, SynthFrame* synthFrame)
	: apvts(apvts), stateParams(stateParams),
	open_btn((const juce::String)"openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	play_btn((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	stop_btn((const juce::String)"stopFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	isAudioLoaded(stateParams->getAudioLoaded()), thumbnailCache(5),
	loader(stateParams, &thumbnailComponent), thumbnailComponent(5, *loader.getFormatManager(), thumbnailCache) //, stateParams)
{
	synthFrame->init(&thumbnailComponent);

	setOpenButtonImageOpen();
	setStopButtonImageStop();
	setPlayButtonImagePlay();

	open_btn.onClick = [this]() {
		openFileButtonClicked();
	};
	stop_btn.onClick = [this]() {
		stopFileButtonClicked();
	};
	play_btn.onClick = [this]() {
		playFileButtonClicked();
	};

	//addAndMakeVisible(&audioFileComponent);
	addAndMakeVisible(&open_btn);
	addAndMakeVisible(&play_btn);
	addAndMakeVisible(&stop_btn);
	addAndMakeVisible(&thumbnailComponent);

	//play_btn.setToggleState(false, juce::NotificationType::dontSendNotification);
	play_btn.setEnabled(false);

	// on écoute la value dans le stateParams
	isAudioLoaded->addListener(this);
}

AudioFileFrame::~AudioFileFrame()
{
	//open_btn.removeListener(this);
	//play_btn.removeListener(this);
	//stop_btn.removeListener(this);
	isAudioLoaded->removeListener(this);

	//loader.reset();
	apvts = nullptr;
	stateParams = nullptr;
}

void AudioFileFrame::openFileButtonClicked()
{
	//stateParams->setAudioLoaded(false);
	loader.loadFile();
	//loader->loadFile();
}

void AudioFileFrame::stopFileButtonClicked()
{
	stateParams->setIsPlaying(false);
}

void AudioFileFrame::playFileButtonClicked()
{
	if (stateParams->getIsPlaying() == true)
	{
		stateParams->setIsPlaying(false);
		setPlayButtonImagePause();
	}
	else {
		stateParams->setIsPlaying(true);
		setPlayButtonImagePlay();
	}

}

void AudioFileFrame::setPlayButtonImagePlay()
{
	play_btn.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::Play_svg,
			BinaryData::Play_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::Play_Fill_svg,
			BinaryData::Play_Fill_svgSize).get(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	);
}

void AudioFileFrame::setPlayButtonImagePause()
{
	stateParams->setIsPlaying(false);
	play_btn.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::Pause_svg,
			BinaryData::Pause_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::Pause_Fill_svg,
			BinaryData::Pause_Fill_svgSize).get(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	);
}

void AudioFileFrame::setOpenButtonImageOpen()
{
	open_btn.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_svg,
			BinaryData::AddFolder_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_Fill_svg,
			BinaryData::AddFolder_Fill_svgSize).get(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	);
}

void AudioFileFrame::setStopButtonImageStop()
{
	stop_btn.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::Stop_svg,
			BinaryData::Stop_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::Stop_Fill_svg,
			BinaryData::Stop_Fill_svgSize).get(),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	);
}

// si le boolean à changer alors on execute cette fonction.
void AudioFileFrame::valueChanged(juce::Value& value)
{

	isAudioLoaded->setValue(value);

	if (value == true)
	{
		//DBG("isAudioLoaded = true");

		// on test le buffer
		if (stateParams->getAudioBuffer() == nullptr)
		{
			return;
		}

		//alors on peut activer le bouton play
		//play_btn.setToggleState(true, juce::NotificationType::dontSendNotification);
		play_btn.setEnabled(true);
	}
	else
	{
		//DBG("isAudioLoaded = false");

		// on ne peut pas play le son mais on ne laisse le btn play
	}
}

// marche pas

bool AudioFileFrame::isInterestedInFileDrag(const juce::StringArray& files)
{
	// is it an audio file ?
	for (juce::String file : files) {
		if (file.contains(".wav") || (".aif") || (".mp3"))
		{
			return true;
		}
	}
	return false;
}

void AudioFileFrame::filesDropped(const juce::StringArray& files, int x, int y)
{
	for (juce::String file : files) {
		if (isInterestedInFileDrag(file)) {
			loader.loadFile(file);
		}
	}
}

// component section

void AudioFileFrame::paint(juce::Graphics& g) {
	g.fillAll(MyColours::brightBlue);
}

void AudioFileFrame::resized() {

	juce::Rectangle<int> localArea = getLocalBounds();
	float w = getWidth() / 30.f;
	float h = getHeight() / 30.f;

	localArea.removeFromTop(static_cast<int>(h));
	localArea.removeFromBottom(static_cast<int>(h));
	//audioFileComponent.setBounds(area.removeFromLeft(static_cast<int>(w)));
	juce::Rectangle<int> buttonsArea = localArea.removeFromLeft(static_cast<int>(w));
	localArea.removeFromLeft(static_cast<int>(h));
	localArea.removeFromRight(static_cast<int>(h));

	juce::Rectangle<int> SpectrumArea = localArea.removeFromLeft(localArea.getWidth());

	// on déclare les flexbox	
	juce::FlexBox flexboxMain;
	flexboxMain.flexDirection = juce::FlexBox::Direction::row;

	juce::FlexBox flexboxLeft;
	flexboxLeft.flexDirection = juce::FlexBox::Direction::column;
	flexboxLeft.flexWrap = juce::FlexBox::Wrap::noWrap;
	flexboxLeft.alignContent = juce::FlexBox::AlignContent::stretch;
	flexboxLeft.alignItems = juce::FlexBox::AlignItems::stretch;
	flexboxLeft.justifyContent = juce::FlexBox::JustifyContent::center;


	juce::FlexBox flexboxRight;
	flexboxRight.flexDirection = juce::FlexBox::Direction::column;
	flexboxRight.flexWrap = juce::FlexBox::Wrap::noWrap;
	flexboxRight.alignContent = juce::FlexBox::AlignContent::stretch;
	flexboxRight.alignItems = juce::FlexBox::AlignItems::stretch;



	// on ajoute les items dans les flexbox

	flexboxLeft.items.add(juce::FlexItem(open_btn).withHeight(buttonsArea.getWidth() * 2));
	flexboxLeft.items.add(juce::FlexItem(play_btn).withHeight(buttonsArea.getWidth() * 1.33));
	//flexboxLeft.items.add(juce::FlexItem(stop_btn).withHeight(buttonsArea.getWidth()));

	flexboxRight.items.add(juce::FlexItem(thumbnailComponent).withHeight(SpectrumArea.getHeight()));

	flexboxMain.items.add(juce::FlexItem(flexboxLeft).withFlex(0.05).withMargin(h));
	flexboxMain.items.add(juce::FlexItem(flexboxRight).withFlex(0.95).withMargin(h));
	flexboxMain.performLayout(getLocalBounds().toFloat());

}

//void AudioFileComponent::resized() {
//	juce::Rectangle<int> area = getLocalBounds();
//	int h = getHeight() / 3;
//
//	open_btn->setBounds(area.removeFromTop(h));
//	play_btn->setBounds(area.removeFromTop(h));
//	stop_btn->setBounds(area.removeFromTop(h));
//}