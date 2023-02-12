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
	//this->stateSaver = stateSaver;
	formatManager.registerBasicFormats();
	buffer = new juce::AudioBuffer<float>();
}

AudioFileLoader::~AudioFileLoader()
{
	delete buffer;
}

// https://forum.juce.com/t/solved-juce-filechooser-has-no-member-browseforfiletoopen/47793/3
// https://forum.juce.com/t/filechooser-not-appearing-in-windows/54952/2
void AudioFileLoader::openFile()
{
	// Choose the file to import
	juce::FileChooser* chooser = new juce::FileChooser
		("Select a audio file to play...", juce::File{}, formatManager.getWildcardForAllFormats());

	auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

	// TODO : check pour quoi le programme ne rentre pas dans le lambda ???
	// lambda function to analyse the sound from the choosen file
	chooser->launchAsync(flags, [this](const juce::FileChooser& chooser)
	{
		juce::File file = chooser.getResult();
		if (file.exists())
		{
			juce::Logger::outputDebugString("fichier exist!");
			this->loadAudio(file);
		}
	});
}

//https://forum.juce.com/t/load-binary-wav-files-into-audiosamplebuffer-array/38790
// On veut charger le son dans le buffer du stateSaver.
void AudioFileLoader::loadAudio(juce::File file) 
{
	juce::AudioFormatReader* reader = formatManager.createReaderFor(file);

	if (reader != nullptr)
	{
		buffer->setSize(reader->numChannels, reader->lengthInSamples);
	}

}