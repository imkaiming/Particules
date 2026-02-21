/*
  ==============================================================================

	AudioFileLoader.cpp
	Created: 3 Feb 2023 12:43:18pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileLoader.h"
#include "../Framework/ParameterView.h"
#include "../Framework/Core.h"

AudioFileLoader::AudioFileLoader(ParameterView& paramsView, ThumbnailComponent& thumbnailComponent):
	paramsView{paramsView}, thumbnailComponent{thumbnailComponent}
{
	// permet au manager de format de gérer les formats WAV, AIFF, MP3, etc.
	formatManager.registerBasicFormats();
}

// https://forum.juce.com/t/solved-juce-filechooser-has-no-member-browseforfiletoopen/47793/3
// https://forum.juce.com/t/filechooser-not-appearing-in-windows/54952/2
void AudioFileLoader::loadFile()
{
	if(!chooser)
		chooser = std::make_unique<juce::FileChooser>("Select an audio file.", juce::File{}, formatManager.getWildcardForAllFormats());

	int flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

	//juce::Logger::outputDebugString("chooser.launchAsync(flags, [this](const juce::FileChooser& resultChooser) ");
	chooser->launchAsync(flags, [this](const juce::FileChooser& resultChooser)
	{
		juce::File file = resultChooser.getResult();
		bool ok = false;
		if(file.existsAsFile())
		{
			ok = this->loadAudio(file);
		}

		if(onFileLoaded)
		{
			onFileLoaded(ok);
		}
	});
}

// https://www.youtube.com/watch?v=2OErY-qhGyw
void AudioFileLoader::loadFile(const juce::String& path)
{

	//juce::Logger::outputDebugString("File dragged ! ");
	juce::File file(path);
	bool ok = false;
	if(file.existsAsFile())
	{
		ok = this->loadAudio(file);
	}
	if(onFileLoaded)
	{
		onFileLoaded(ok);
	}
}

bool AudioFileLoader::loadAudio(juce::File& file)
{
	std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

	if(!reader)
		return false;

	// si le reader de fichier lit un fichier de plus de MaxDuration alors on annule tout
	const double fileDuration = static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
	if(fileDuration >= static_cast<double>(maxDuration))
	{
		showErrorWindow("The file duration must not exceed " + (juce::String)maxDuration + " seconds." + "/nYour file is currently " + juce::String(fileDuration, 2) + " seconds.");
		return false;
	}

	const double targetSampleRate = paramsView.getSampleRate();
	if(targetSampleRate <= 0.0)
	{
		showErrorWindow("invalid sample rate : targetSampleRate <= 0");
		return false;
	}

	const int numSamples = (int)reader->lengthInSamples;
	const unsigned int numChannels = reader->numChannels;

	if(file.getSize() > maxFileSize)
	{
		showErrorWindow("File too large to process (" + juce::String(file.getSize() / (1024 * 1024)) + " MB estimated)");
		return false;
	}

	double ratio = reader->sampleRate / targetSampleRate;

	juce::AudioBuffer<float> tempBuffer(numChannels, numSamples);
	if(!reader->read(&tempBuffer, 0, numSamples, 0, true, true))
	{
		showErrorWindow("Failed to read audio data from the temporary buffer");
		return false;
	}

	const int resampledSamples = static_cast<int> (numSamples / ratio);

	juce::AudioBuffer<float> resampledBuffer(numChannels, resampledSamples);

	juce::LagrangeInterpolator resampler;

	for(int channel = 0; channel < resampledBuffer.getNumChannels(); ++channel)
	{
		resampler.reset();
		resampler.process(ratio,
						  tempBuffer.getReadPointer(channel),
						  resampledBuffer.getWritePointer(channel),
						  resampledSamples);
	}

	thumbnailComponent.setFile(file);

	std::shared_ptr<const SampleSource> source = std::make_shared<const SampleSource>(std::move(resampledBuffer), paramsView.getSampleRate());

	//const SampleSource source(resampledBuffer, targetSampleRate);
	paramsView.setSampleSource(source);
	paramsView.getGrainVisualizer()->setGrains(paramsView.getGrains());

	return true;
}

void AudioFileLoader::setOnFileLoadedCallBack(std::function<void(bool)> callbackOnFileLoaded)
{
	onFileLoaded = std::move(callbackOnFileLoaded);
}

juce::AudioFormatManager& AudioFileLoader::getFormatManager()
{
	return formatManager;
}

void AudioFileLoader::showErrorWindow(juce::String message)
{
	juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon, "Error", message, "OK");
}