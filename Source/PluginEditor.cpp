#include "PluginProcessor.h"
#include "PluginEditor.h"

ParticulesAudioProcessorEditor::ParticulesAudioProcessorEditor(
	ParticulesAudioProcessor& p) : AudioProcessorEditor(&p), audioProcessor(p), 
	mainFrame(p.getValueTreeState(), p.getStateParameters())
{

	this->setLookAndFeel(&customLookAndFeel);

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

