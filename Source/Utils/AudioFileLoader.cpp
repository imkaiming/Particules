/*
  ==============================================================================

	AudioFileLoader.cpp
	Created: 3 Feb 2023 12:43:18pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileLoader.h"

//AudioFileLoader* AudioFileLoader::instance = 0;

AudioFileLoader::AudioFileLoader(StateParameters* stateParams, ThumbnailComponent* thumbnailComponent) :
	stateParams(stateParams), thumbnailComponent(thumbnailComponent)
{
	readerSource = new std::unique_ptr<juce::AudioFormatReaderSource>();
	// permet au manager de format de gérer les formats WAV, AIFF, MP3, etc.
	formatManager.registerBasicFormats();

}

AudioFileLoader::~AudioFileLoader()
{
	stateParams = nullptr;
	readerSource->reset(nullptr);
}

// https://forum.juce.com/t/load-binary-wav-files-into-audiosamplebuffer-array/38790
// https://forum.juce.com/t/resampling-an-audiosamplebuffer/14287/5 
void AudioFileLoader::loadAudio(juce::File& file)
{
	juce::AudioFormatReader* reader = formatManager.createReaderFor(file);

	if (reader == nullptr)
	{
		juce::Logger::outputDebugString("reader == nullptr ");
		return;
	}

	// si le reader de fichier lit un fichier de plus de MAX_DURATION alors on annule tout
	if (reader->lengthInSamples / reader->sampleRate >= MAX_DURATION)
	{
		//stateParams->setAudioBuffer(nullptr);
		//stateParams->setAudioLoaded(false);
		showErrorWindow("The file duration must not exceed " + (juce::String)MAX_DURATION + " seconds.");
		return;
	}

	std::unique_ptr<juce::AudioFormatReaderSource> newSource(
		new juce::AudioFormatReaderSource(reader, true)
	);

	juce::AudioBuffer<float>* buffer = new juce::AudioBuffer<float>();
	readerSource->reset(newSource.release());
	thumbnailComponent->setFile(file);

	// on procède au resampling de la source par rapport à la fréquence d'échantillonage du projet

	const int numSamples = reader->lengthInSamples;
	const int numChannels = reader->numChannels;
	double ratio = reader->sampleRate / stateParams->getSampleRate();

	juce::AudioBuffer<float> tempBuffer(numChannels, numSamples);
	juce::AudioBuffer<float> newBuffer(numChannels, (int)((double)numSamples / ratio));

	reader->read(&tempBuffer, 0, numSamples,
		0, true, true);

	std::unique_ptr<juce::LagrangeInterpolator> resampler(new juce::LagrangeInterpolator());

	const float** inputs = (const float**)tempBuffer.getArrayOfReadPointers();
	float** outputs = (float**)newBuffer.getArrayOfWritePointers();

	for (int channel = 0; channel < newBuffer.getNumChannels(); ++channel)
	{
		resampler->reset();
		resampler->process(ratio, inputs[channel],
			outputs[channel], newBuffer.getNumSamples());
	}

	*buffer = juce::AudioBuffer<float>(newBuffer);

	stateParams->setAudioBuffer(buffer);
	stateParams->setAudioLoaded(true);
	stateParams->getGrainVisualizer()->setGrains(stateParams->getGrains());



}

// https://forum.juce.com/t/solved-juce-filechooser-has-no-member-browseforfiletoopen/47793/3
// https://forum.juce.com/t/filechooser-not-appearing-in-windows/54952/2
void AudioFileLoader::loadFile()
{
	if (stateParams == nullptr) {
		return;
	}

	juce::FileChooser* chooser = new juce::FileChooser
	("Select an audio file.", juce::File{}, formatManager.getWildcardForAllFormats());

	int flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

	chooser->launchAsync(flags, [this](const juce::FileChooser& chooser)
		{
			juce::File file = chooser.getResult();
	if (file.exists())
	{
		this->loadAudio(file);
	}
		});
}

// https://www.youtube.com/watch?v=2OErY-qhGyw
void AudioFileLoader::loadFile(const juce::String& path) {

	juce::Logger::outputDebugString("File dragged ! ");
	juce::File file(path);

	if (stateParams == nullptr) {
		return;
		juce::Logger::outputDebugString("Le statesaver n'est pas initialisé dans AudioFileLoader.");
	}

	if (file.exists()) {
		this->loadAudio(file);
	}
}

juce::AudioFormatManager* AudioFileLoader::getFormatManager()
{
	return &formatManager;
}


void AudioFileLoader::showErrorWindow(juce::String message)
{
	juce::AlertWindow::showMessageBoxAsync(
		juce::AlertWindow::AlertIconType::WarningIcon,
		"Error",
		message,
		"OK");
}