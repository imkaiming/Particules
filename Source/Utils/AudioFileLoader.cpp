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
	readerSource.reset();
	//delete buffer;
}

// https://forum.juce.com/t/solved-juce-filechooser-has-no-member-browseforfiletoopen/47793/3
// https://forum.juce.com/t/filechooser-not-appearing-in-windows/54952/2
void AudioFileLoader::openFile(juce::AudioBuffer<float>* buffer)
{

	juce::FileChooser* chooser = new juce::FileChooser
	("Select a audio file to play...", juce::File{}, formatManager.getWildcardForAllFormats());

	auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

	chooser->launchAsync(flags, [this, &buffer](const juce::FileChooser& chooser)
	{
		juce::File file = chooser.getResult();
		if (file.exists())
		{
			juce::Logger::outputDebugString("Le fichier existe!");
			this->loadAudio(file, buffer);
		}
	});

}

//https://forum.juce.com/t/load-binary-wav-files-into-audiosamplebuffer-array/38790
// On veut charger le son dans le buffer du pluginprocessor.
void AudioFileLoader::loadAudio(juce::File file, juce::AudioBuffer<float>* buffer)
{
	juce::AudioFormatReader* reader = formatManager.createReaderFor(file);

	juce::Logger::outputDebugString((const juce::String) reader->sampleRate);
	if (reader != nullptr)
	{
		buffer->setSize(reader->numChannels, reader->lengthInSamples);
	}


}