/*
  ==============================================================================

	AudioFileLoader.cpp
	Created: 3 Feb 2023 12:43:18pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileLoader.h"

AudioFileLoader::AudioFileLoader()
{
	formatManager.registerBasicFormats();
}

AudioFileLoader::~AudioFileLoader()
{

}

void AudioFileLoader::loadAudioFile() {

	juce::FileChooser chooser("Choose a WAV or AIFF file.", juce::File{}, "*.wav", " * .aiff", true, false);

	auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

	chooser.launchAsync(chooserFlags, [this](const juce::FileChooser& fc) {
		juce::File file = fc.getResult();

	if (file != juce::File{})
	{
		juce::AudioFormatReader* audioFormatReader = formatManager.createReaderFor(file);

		if (audioFormatReader != nullptr)
		{
			auto newSource = std::make_unique<juce::AudioFormatReaderSource>(audioFormatReader, true);
			//transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
			transportSource.setSource(newSource.get(), 0, nullptr, audioFormatReader->sampleRate);

			//playFileButton.setEnabled(true);
			readerSource.reset(newSource.release());
		}
	}

		});
}

//void AudioFileLoader::init(StateSaver* stateSaver) {
//	this->stateSaver = stateSaver;
//}