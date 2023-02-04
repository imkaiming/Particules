#include "PluginProcessor.h"
#include "PluginEditor.h"

GranularSynthesisAudioProcessorEditor::GranularSynthesisAudioProcessorEditor(GranularSynthesisAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.

	this->setLookAndFeel(&customLookAndFeel);
	

	setResizable(true, true);
	setResizeLimits(450, 225, 1200, 600);

	const float ratio = 2.f;
	getConstrainer()->setFixedAspectRatio(ratio);

	width = 900;
	heigth = 450;
	setSize(width, heigth);

	addAndMakeVisible(mainFrame);

	//juce::Typeface::createSystemTypefaceFor(
	//	BinaryData::CascadiaMonoPLRegular_otf,
	//	BinaryData::CascadiaMonoPLRegular_otfSize));
}

GranularSynthesisAudioProcessorEditor::~GranularSynthesisAudioProcessorEditor()
{
	this->setLookAndFeel(nullptr);
}

void GranularSynthesisAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	// g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	g.fillAll(juce::Colours::black);
	// g.setColour(juce::Colours::white);
	// g.setFont(15.0f);
	// g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void GranularSynthesisAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	//int w = getWidth();
	//int h = getHeight();
	//juce::Rectangle<int> bounds(w - 5, h);
	int w = getWidth() / 300;
	juce::Rectangle<int> area = getLocalBounds();
	area.removeFromLeft(w);
	area.removeFromRight(w);
	mainFrame.setBounds(area);
}
