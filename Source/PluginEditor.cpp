#include "PluginProcessor.h"
#include "PluginEditor.h"

ParticulesAudioProcessorEditor::ParticulesAudioProcessorEditor(
	ParticulesAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p),
	open_btn((const juce::String)"openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	play_btn((const juce::String)"saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	stop_btn((const juce::String)"stopFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
	mainFrame(&open_btn, &play_btn, &stop_btn)
{
	

	initButtons();
	loader = std::make_unique<AudioFileLoader>();


	this->setLookAndFeel(&customLookAndFeel);
	this->stateSaver = audioProcessor.getStateSaver();

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
	juce::Logger::outputDebugString("openFileButtonClicked");
	// TODO : il faut stoper la lecture du plugin si elle est en cours
	juce::AudioBuffer<float>* buffer = audioProcessor.getSampleBuffer();
	buffer->clear();
	stateSaver->setAudioLoaded(false);
	loader->openFile(buffer);
	if (buffer->getNumSamples() > 0) {
		juce::Logger::outputDebugString("Le buffer est chargé en samples");
		stateSaver->setAudioLoaded(true);
	}

}

void ParticulesAudioProcessorEditor::stopFileButtonClicked()
{
	juce::Logger::outputDebugString("stopFileButtonClicked");
}


void ParticulesAudioProcessorEditor::playFileButtonClicked()
{
	juce::Logger::outputDebugString("playFileButtonClicked");

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