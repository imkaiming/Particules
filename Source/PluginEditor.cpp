#include "PluginProcessor.h"
#include "PluginEditor.h"

ParticulesAudioProcessorEditor::ParticulesAudioProcessorEditor(
	ParticulesAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p),
	stateSaver(p.getStateSaver()),
	open_btn((const juce::String)"openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	play_btn((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	stop_btn((const juce::String)"stopFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	mainFrame(&open_btn, &play_btn, &stop_btn)
{
	loader = std::make_unique<AudioFileLoader>(stateSaver);
	initButtons();


	this->setLookAndFeel(&customLookAndFeel);
	//this->stateSaver = audioProcessor.getStateSaver();

	setResizable(true, true);
	setResizeLimits(450, 225, 1200, 600);

	const float ratio = 2.f;
	getConstrainer()->setFixedAspectRatio(ratio);

	width = 900;
	heigth = 450;
	setSize(width, heigth);

	addAndMakeVisible(&mainFrame);

	//juce::Typeface::createSystemTypefaceFor(
	//	BinaryData::CascadiaMonoPLRegular_otf,
	//	BinaryData::CascadiaMonoPLRegular_otfSize));
}

ParticulesAudioProcessorEditor::~ParticulesAudioProcessorEditor()
{
	loader.reset();
	this->setLookAndFeel(nullptr);
	this->stateSaver = nullptr;
}

void ParticulesAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::black);
}

void ParticulesAudioProcessorEditor::resized()
{
	int w = getWidth() / 300;
	juce::Rectangle<int> area = getLocalBounds();
	area.removeFromLeft(w);
	area.removeFromRight(w);
	mainFrame.setBounds(area);
}

void ParticulesAudioProcessorEditor::openFileButtonClicked()
{
	//juce::Logger::outputDebugString("openFileButtonClicked");
	// TODO : il faut stoper la lecture du plugin si elle est en cours
	//buffer->clear();

	stateSaver->setAudioLoaded(false);
	loader->loadFile();
	//buffer = loader->getBuffer();
	//if (buffer->getNumSamples() > 0) {
	//	stateSaver->setAudioLoaded(true);
	//	juce::Logger::outputDebugString("Le buffer de l'éditeur est chargé!");

	//	// on dessine la waveform
	//}
}

void ParticulesAudioProcessorEditor::stopFileButtonClicked()
{
	juce::Logger::outputDebugString("stopFileButtonClicked");
}


void ParticulesAudioProcessorEditor::playFileButtonClicked()
{
	juce::Logger::outputDebugString("playFileButtonClicked");
	//audioProcessor.processBlock();

}

void ParticulesAudioProcessorEditor::initButtons() {

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

bool ParticulesAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
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

void ParticulesAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
	for (juce::String file : files) {
		if (isInterestedInFileDrag(file)) {
			// load this file.
			//loader->loadFile(file);
		}
	}
}