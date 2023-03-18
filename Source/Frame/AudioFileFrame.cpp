/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"

AudioFileFrame::AudioFileFrame(ValueTreeState* apvts, StateParameters* stateParams) : apvts(apvts), stateParams(stateParams),
	open_btn((const juce::String)"openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	play_btn((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	stop_btn((const juce::String)"stopFileButton", juce::DrawableButton::ButtonStyle::ImageFitted)// ,	audioFileComponent()
{

	loader = std::make_unique<AudioFileLoader>(stateParams);
	initDrawableButtons();

	//addAndMakeVisible(&audioFileComponent);
	addAndMakeVisible(this->open_btn);
	addAndMakeVisible(this->play_btn);
	addAndMakeVisible(this->stop_btn);

	addAndMakeVisible(&spectrumComponent);
}

AudioFileFrame::~AudioFileFrame() 
{
	loader.reset();
	apvts = nullptr;
	stateParams = nullptr;
}

void AudioFileFrame::paint(juce::Graphics& g) {
	g.fillAll(juce::Colours::coral);
}
void AudioFileFrame::resized() {
	juce::Rectangle<int> area = getLocalBounds();
	float w = getWidth() / 30.f;
	float h = getHeight() / 30.f;

	area.removeFromTop(static_cast<int>(h));
	area.removeFromBottom(static_cast<int>(h));
	//audioFileComponent.setBounds(area.removeFromLeft(static_cast<int>(w)));
	area.removeFromLeft(static_cast<int>(h));
	area.removeFromRight(static_cast<int>(h));
	spectrumComponent.setBounds(area.removeFromLeft(area.getWidth()));

}

void AudioFileFrame::openFileButtonClicked()
{
	juce::Logger::outputDebugString("openFileButtonClicked");
	// TODO : il faut stoper la lecture du plugin si elle est en cours
	//buffer->clear();

	stateParams->setAudioLoaded(false);
	loader->loadFile();
	//buffer = loader->getBuffer();
	if (stateParams->getAudioLoaded()) {
		//	stateSaver->setAudioLoaded(true);
		juce::Logger::outputDebugString("Le buffer de l'éditeur est chargé!");

		//	// on dessine la waveform
	}
}

void AudioFileFrame::stopFileButtonClicked()
{
	juce::Logger::outputDebugString("stopFileButtonClicked");
}


void AudioFileFrame::playFileButtonClicked()
{
	juce::Logger::outputDebugString("playFileButtonClicked");
	//audioProcessor.processBlock();

}

void AudioFileFrame::initDrawableButtons() {

	open_btn.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_svg, BinaryData::AddFolder_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_Fill_svg, BinaryData::AddFolder_Fill_svgSize).get(),
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	);

	stop_btn.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::Stop_svg, BinaryData::Stop_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::Stop_Fill_svg, BinaryData::Stop_Fill_svgSize).get(),
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	);

	play_btn.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::Play_svg, BinaryData::Play_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::Play_Fill_svg, BinaryData::Play_Fill_svgSize).get(),
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	);

	open_btn.onClick = [this] {
		openFileButtonClicked();
	};
	stop_btn.onClick = [this] {
		stopFileButtonClicked();
	};

	play_btn.onClick = [this] {
		playFileButtonClicked();
	};
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
			// load this file.
			//loader->loadFile(file);
		}
	}
}