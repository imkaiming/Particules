/*
  ==============================================================================

	AudioFileLoader.cpp
	Created: 3 Feb 2023 12:43:18pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileLoader.h"

//AudioFileLoader* AudioFileLoader::instance = 0;

AudioFileLoader::AudioFileLoader(StateParameters* stateParams) : stateParams(stateParams)
{
	readerSource = new std::unique_ptr<juce::AudioFormatReaderSource>();
	// permet au manager de format de gérer les formats WAV, AIFF, MP3, etc.
	formatManager.registerBasicFormats();
	buffer = new juce::AudioBuffer<float>();
	//reader = nullptr;
}

AudioFileLoader::~AudioFileLoader()
{
	this->unloadFile();
	stateParams = nullptr;
	delete buffer;
	readerSource->reset(nullptr);
}

void AudioFileLoader::unloadFile()
{
	buffer->clear();
	stateParams->setAudioLoaded(false);
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

	std::unique_ptr<juce::AudioFormatReaderSource> newSource(
		new juce::AudioFormatReaderSource(reader, true)
	);

	this->unloadFile();
	delete buffer;
	buffer = new juce::AudioBuffer<float>();

	readerSource->reset(newSource.release());

	const int numSamples = reader->lengthInSamples;
	const int numChannels = reader->numChannels;
	double ratio = reader->sampleRate / stateParams->getSampleRate();

	//juce::Logger::outputDebugString("numSamples : " + (juce::String)numSamples);
	//juce::Logger::outputDebugString("numChannels : " + (juce::String)numChannels);
	//juce::Logger::outputDebugString("reader->sampleRate : " + (juce::String)reader->sampleRate);
	//juce::Logger::outputDebugString("stateSaver->getSampleRate: " + (juce::String)stateParams->getSampleRate());

	//buffer.setSize(numChannels, numSamples);
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

	juce::Logger::outputDebugString("buffer.getNumSamples() : " + (juce::String)this->buffer->getNumSamples());
	*this->buffer = juce::AudioBuffer<float>(newBuffer);
	juce::Logger::outputDebugString("buffer.getNumSamples() : " + (juce::String)this->buffer->getNumSamples());

	stateParams->setAudioBuffer(buffer);
	stateParams->setAudioLoaded(true);

	//reader = nullptr;
}

// https://forum.juce.com/t/solved-juce-filechooser-has-no-member-browseforfiletoopen/47793/3
// https://forum.juce.com/t/filechooser-not-appearing-in-windows/54952/2
void AudioFileLoader::loadFile()
{
	if (stateParams == nullptr) {
		return;
		juce::Logger::outputDebugString("Le statesaver n'est pas initialisé dans AudioFileLoader.");
	}

	juce::FileChooser* chooser = new juce::FileChooser
	("Select an audio file.", juce::File{}, formatManager.getWildcardForAllFormats());

	auto flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

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


juce::AudioBuffer<float>* AudioFileLoader::getAudioBuffer() const {
	return this->buffer;
}