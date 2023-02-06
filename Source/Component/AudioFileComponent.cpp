
#include "AudioFileComponent.h"

AudioFileComponent::AudioFileComponent() :
	openFileButton((const juce::String)"openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	stopFileButton((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	playFileButton((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted)
{

	openFileButton.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_svg, BinaryData::AddFolder_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_Fill_svg, BinaryData::AddFolder_Fill_svgSize).get(),
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	);

	stopFileButton.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::Stop_svg, BinaryData::Stop_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::Stop_Fill_svg, BinaryData::Stop_Fill_svgSize).get(),
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	);

	playFileButton.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::Play_svg, BinaryData::Play_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::Play_Fill_svg, BinaryData::Play_Fill_svgSize).get(),
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	);

	openFileButton.onClick = [this] {
		openFileButtonClicked();
	};
	stopFileButton.onClick = [this] {
		stopFileButtonClicked();
	};

	playFileButton.onClick = [this] {
		playFileButtonClicked();
	};

	addAndMakeVisible(&openFileButton);
	addAndMakeVisible(&playFileButton);
	addAndMakeVisible(&stopFileButton);

	// This allow us to manage WAV and AIFF files
	//formatManager.registerBasicFormats();
	//transportSource.addChangeListener(this);
}

AudioFileComponent::~AudioFileComponent() {

}

void AudioFileComponent::paint(juce::Graphics&) {

}

void AudioFileComponent::resized() {
	juce::Rectangle<int> area = getLocalBounds();
	int h = getHeight() / 3;

	openFileButton.setBounds(area.removeFromTop(h));
	playFileButton.setBounds(area.removeFromTop(h));
	stopFileButton.setBounds(area.removeFromTop(h));
}

void AudioFileComponent::openFileButtonClicked()
{

	juce::Logger::outputDebugString("openFileButtonClicked");



}

void AudioFileComponent::stopFileButtonClicked()
{
	juce::Logger::outputDebugString("stopFileButtonClicked");
	juce::Logger::outputDebugString(getParentComponent()->getName());
}


void AudioFileComponent::playFileButtonClicked()
{
	juce::Logger::outputDebugString("playFileButtonClicked");
	juce::Logger::outputDebugString(getParentComponent()->getName());

}

//void AudioFileComponent::changeState(TransportState newState)
//{
//	if (state != newState)
//	{
//		state = newState;
//
//		switch (state)
//		{
//		case Stopped:
//			stopFileButton.setEnabled(false);
//			playFileButton.setEnabled(true);
//			transportSource.setPosition(0.0);
//			break;
//
//		case Starting:
//			playFileButton.setEnabled(false);
//			transportSource.start();
//			break;
//
//		case Playing:
//			stopFileButton.setEnabled(true);
//			break;
//
//		case Stopping:
//			transportSource.stop();
//			break;
//		}
//	}
//}

//void AudioFileComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
//{
//	if (source == &transportSource)
//	{
//		if (transportSource.isPlaying())
//			changeState(Playing);
//		else
//			changeState(Stopped);
//	}
//}

//TransportState AudioFileComponent::getCurrentState() {
//	return state;
//}
//
//std::unique_ptr<juce::AudioFormatReaderSource> AudioFileComponent::getReaderSource() {
//	return readerSource;
//}
//
//juce::AudioTransportSource AudioFileComponent::getTransportSource() {
//	return transportSource;
//}

void AudioFileComponent::init(StateSaver* stateSaver) {
	this->stateSaver = stateSaver;
}

void AudioFileComponent::loadAudioFile()
{

}