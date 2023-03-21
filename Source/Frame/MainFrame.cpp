/*
  ==============================================================================

	MainFrame.cpp
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#include "MainFrame.h"

MainFrame::MainFrame(ValueTreeState* apvts, StateParameters* stateParams) : 
	titleFrame(), synthFrame(apvts, stateParams), 
	grainsFrame(apvts, stateParams), audioFileFrame(apvts, stateParams, &synthFrame), 
	statusBarFrame(stateParams)
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
	//juce::Rectangle<float> mainFrame(5.f, 0.f, getWidth() - 10.f, getHeight());
	//g.setColour(juce::Colours::grey);
	//g.fillRect(mainFrame);
	//g.drawRect(mainFrame);
	//g.fillAll(juce)

}

void MainFrame::resized()
{
	//juce::Rectangle<int> area = getLocalBounds();
	float heightComp = getHeight() / 30.f;
	float widthComp = getWidth() / 10.f;

	//float w = getWidth() / 300.f;

	// old positionning V2
	//titleFrame.setBounds(area.removeFromTop(static_cast<int>(heightComp * 1)));
	//area.removeFromTop(static_cast<int>(w));
	//synthFrame.setBounds(area.removeFromTop(static_cast<int>(heightComp * 9)));
	//area.removeFromTop(static_cast<int>(w));
	//grainsFrame.setBounds(area.removeFromTop(static_cast<int>(heightComp * 9)));
	//area.removeFromTop(static_cast<int>(w));
	//audioFileFrame.setBounds(area.removeFromTop(static_cast<int>(heightComp * 9)));
	//area.removeFromTop(static_cast<int>(w));
	//statusBarFrame.setBounds(area.removeFromTop(static_cast<int>(heightComp * 2)));


	// old positionning
	//titleFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 1);
	//statusBarFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 9);
	//synthFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 9);
	//grainsFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 9);
	//audioFileFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 2);

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

