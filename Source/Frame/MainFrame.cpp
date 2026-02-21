/*
  ==============================================================================

	MainFrame.cpp
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#include "MainFrame.h"
#include "../Framework/ParameterView.h"
#include "../Framework/UIContext.h"
#include "../Utils/MyColours.h"


MainFrame::MainFrame(UIContext& uic):	titleFrame(uic.customLookAndFeel), synthFrame(uic.apvts), 
grainsFrame(uic.apvts), audioFileFrame(uic), statusBarFrame()
{
	addAndMakeVisible(&titleFrame);
	addAndMakeVisible(&synthFrame);
	addAndMakeVisible(&grainsFrame);
	addAndMakeVisible(&audioFileFrame);
	addAndMakeVisible(&statusBarFrame);
}

MainFrame::~MainFrame()
{
}

void MainFrame::paint(juce::Graphics& g)
{
	g.fillAll(MyColours::black);
}

void MainFrame::resized()
{
	float heightComp = getHeight() / 30.f;

	juce::FlexBox flexbox;
	flexbox.flexDirection = juce::FlexBox::Direction::column;
	flexbox.flexWrap = juce::FlexBox::Wrap::noWrap;
	flexbox.alignContent = juce::FlexBox::AlignContent::stretch;
	flexbox.alignItems = juce::FlexBox::AlignItems::stretch;

	flexbox.items.add(juce::FlexItem(titleFrame).withHeight(heightComp * 2));
	flexbox.items.add(juce::FlexItem(synthFrame).withHeight(heightComp * 9).withMargin(1));
	flexbox.items.add(juce::FlexItem(grainsFrame).withHeight(heightComp * 9).withMargin(1));
	flexbox.items.add(juce::FlexItem(audioFileFrame).withHeight(heightComp * 9).withMargin(1));
	flexbox.items.add(juce::FlexItem(statusBarFrame).withHeight(heightComp * 1));

	flexbox.performLayout(getLocalBounds().toFloat());
}

