
#include "AudioFileComponent.h"

AudioFileComponent::AudioFileComponent() :
	openFileButton((const juce::String)"openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	saveFileButton((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	playFileButton((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted)
{
	openFileButton.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_svg, BinaryData::AddFolder_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::AddFolder_Fill_svg, BinaryData::AddFolder_Fill_svgSize).get(),
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
	);

	saveFileButton.setImages(
		juce::Drawable::createFromImageData(
			BinaryData::SavePresets_svg, BinaryData::SavePresets_svgSize).get(),
		juce::Drawable::createFromImageData(
			BinaryData::SavePresets_Fill_svg, BinaryData::SavePresets_Fill_svgSize).get(),
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
	saveFileButton.onClick = [this] {
		saveFileButtonClicked();
	};

	playFileButton.onClick = [this] {
		playFileButtonClicked();
	};

	addAndMakeVisible(&openFileButton);
	addAndMakeVisible(&saveFileButton);
	addAndMakeVisible(&playFileButton);

	// This allow us to manage WAV and AIFF files
	audioFormatManager.registerBasicFormats();
}

AudioFileComponent::~AudioFileComponent() {

}

void AudioFileComponent::paint(juce::Graphics&) {

}

void AudioFileComponent::resized() {
	juce::Rectangle<int> area = getLocalBounds();
	int h = getHeight() / 3;

	openFileButton.setBounds(area.removeFromTop(h));
	saveFileButton.setBounds(area.removeFromTop(h));
	playFileButton.setBounds(area.removeFromTop(h));
}

void AudioFileComponent::openFileButtonClicked()
{
	juce::Logger::outputDebugString("openFileButtonClicked");
	//juce::FileChooser fileChooser("Choose a WAV or AIFF file.",
	//	juce::File::getSpecialLocation(juce::File::userMusicDirectory),
	//	"*.wav", "*.aiff", true, false);

	//if (fileChooser.getResult().existsAsFile()) {
	//	juce::File file = fileChooser.getResult();
	//	juce::AudioFormatReader* audioFormatReader = audioFormatManager.createReaderFor(file);
	//	std::unique_ptr<juce::AudioFormatReaderSource> tempSource(
	//		new juce::AudioFormatReaderSource(audioFormatReader, true)
	//	);
	//	juce::Logger::outputDebugString("path of the audio source is : ");
	//	juce::Logger::outputDebugString(file.getFullPathName());
	//}

	juce::FileChooser chooser("Choose a WAV or AIFF file.",
		juce::File{}, "*.wav", " * .aiff", true, false);

	auto chooserFlags = juce::FileBrowserComponent::openMode
		| juce::FileBrowserComponent::canSelectFiles;

	chooser.launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
	{
		juce::File file = fc.getResult();

		if (file != juce::File{})
		{
			juce::AudioFormatReader* audioFormatReader = audioFormatManager.createReaderFor(file); 

			if (audioFormatReader != nullptr)
			{
				//std::unique_ptr<juce::AudioFormatReaderSource> tempSource(
							//new juce::AudioFormatReaderSource(audioFormatReader, true)

				auto newSource = std::make_unique<juce::AudioFormatReaderSource>(audioFormatReader, true);
				transportSource.setSource(newSource.get(), 0, nullptr, audioFormatReader->sampleRate);
				playButton.setEnabled(true);                   
				readerSource.reset(newSource.release());
			}
		}
	});

}

void AudioFileComponent::saveFileButtonClicked()
{
	juce::Logger::outputDebugString("saveFileButtonClicked");



	void AudioFileComponent::playFileButtonClicked()
	{
		juce::Logger::outputDebugString("playFileButtonClicked");

	}