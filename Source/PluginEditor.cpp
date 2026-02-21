#include "PluginProcessor.h"
#include "PluginEditor.h"

ParticulesAudioProcessorEditor::ParticulesAudioProcessorEditor(
	ParticulesAudioProcessor& p): AudioProcessorEditor(&p), audioProcessor(p), mainFrame(p.getUIContext()), customLookAndFeel(p.getUIContext().customLookAndFeel)
{

	this->setLookAndFeel(&customLookAndFeel);

	setResizable(true, true);
	setResizeLimits(450, 225, 1200, 600);

	const float ratio = 2.f;
	getConstrainer()->setFixedAspectRatio(ratio);

	width = 700;
	heigth = 350;
	setSize(width, heigth);

	addAndMakeVisible(&mainFrame);
}

ParticulesAudioProcessorEditor::~ParticulesAudioProcessorEditor()
{
	this->setLookAndFeel(nullptr);
}

void ParticulesAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(MyColours::black);
}

void ParticulesAudioProcessorEditor::resized()
{
	int w = getWidth() / 300;
	juce::Rectangle<int> area = getLocalBounds();
	area.removeFromLeft(w);
	area.removeFromRight(w);
	mainFrame.setBounds(area);
}

