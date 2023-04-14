/*
  ==============================================================================

	GrainVisualizer.cpp
	Created: 12 Apr 2023 1:47:43pm
	Author:  user

  ==============================================================================
*/

#include "GrainVisualizer.h"

GrainVisualizer::GrainVisualizer(juce::Array<Grain*>* grains) : grains(grains)
{
	this->grains = nullptr;
}

GrainVisualizer::~GrainVisualizer()
{
	this->grains = nullptr;
}

//void GrainVisualizer::setGrains(juce::Array<Grain*>* grains)
//{
//	DBG("GrainVisualizer::setGrains");
//	this->grains = grains;
//}

void GrainVisualizer::paint(juce::Graphics& g)
{
	// Clear the background
	g.fillAll(juce::Colours::black);

	// Draw the grains visualization
	//for (const auto& grain : grains)
	//{
	//	// Determine the position, size, and color of the grain based on its properties
	//	// You may want to normalize and scale values based on the component's size and the desired appearance

	//	juce::Rectangle<float> grainRect(grain.x, grain.y, grain.width, grain.height);
	//	juce::Colour grainColor = juce::Colour::fromHSV(grain.hue, grain.saturation, grain.brightness, 1.0f);

	//	// Draw the grain
	//	g.setColour(grainColor);
	//	g.fillRect(grainRect);
	//}
}

void GrainVisualizer::update()
{
	DBG("GrainVisualizer::update()");
	//repaint();
}

void GrainVisualizer::resized()
{
}