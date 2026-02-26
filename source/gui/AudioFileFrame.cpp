/*
  ==============================================================================

	AudioFileFrame.cpp
	Created: 3 Feb 2023 10:38:36pm
	Author:  user

  ==============================================================================
*/

#include "AudioFileFrame.h"
#include "../framework/ParameterView.h"
#include "../framework/UIContext.h"
#include "../pluginProcessor.h"
#include "../utils/MyColours.h"

AudioFileFrame::AudioFileFrame(UIContext& uic)
    : paramsView(uic.paramsView),
      audioProcessor(uic.audioProcessor),
      open_btn((const juce::String) "openFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
      play_btn((const juce::String) "saveFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
      stop_btn((const juce::String) "stopFileButton", juce::DrawableButton::ButtonStyle::ImageFitted),
      thumbnailComponent(5, audioProcessor.getAudioFileLoader().getFormatManager(), uic)
{
    setOpenButtonImageOpen();
    setStopButtonImageStop();
    setPlayButtonImagePlay();

    open_btn.onClick = [this]() { openFileButtonClicked(); };
    stop_btn.onClick = [this]() { stopFileButtonClicked(); };
    play_btn.onClick = [this]() { playFileButtonClicked(); };

    //addAndMakeVisible(&audioFileComponent);
    addAndMakeVisible(&open_btn);
    addAndMakeVisible(&play_btn);
    addAndMakeVisible(&stop_btn);
    addAndMakeVisible(&thumbnailComponent);

    //play_btn.setToggleState(false, juce::NotificationType::dontSendNotification);
    play_btn.setEnabled(false);

    std::function<void()> callbackOnThumbnailReady = [this]() {
        //juce::Logger::outputDebugString("10) callback after thumbnail painted");
        //play_btn.setEnabled(true);
    };

    thumbnailComponent.setCallbackOnThumbnailReady(callbackOnThumbnailReady);

    audioProcessor.addChangeListener(this);
    /*
	// audio file loader is a variable of audio file frame so it cannot outlive his parent.
	std::function<void(bool)> callbackOnFileLoaded = [this](bool ok)
	{
		// UI operation on the message thread
		juce::MessageManager::callAsync([this, ok]()
		{
			if(juce::MessageManager* m = juce::MessageManager::getInstance())
			{
				if(m->currentThreadHasLockedMessageManager())
				{
					//std::shared_ptr<const SampleSource> src = paramsView.getSampleSource();
					play_btn.setEnabled(ok); //&& src);
				}
			}
		});
	};

	audioProcessor.getAudioFileLoader().setOnFileLoadedCallBack(callbackOnFileLoaded);
	*/
}
AudioFileFrame::~AudioFileFrame() { audioProcessor.removeChangeListener(this); }

void AudioFileFrame::openFileButtonClicked()
{
    //juce::Logger::outputDebugString("openFileButtonClicked() ");
    play_btn.setEnabled(false);
    paramsView.setIsPlaying(false);
    audioProcessor.loadFile();

    //(paramsView.getAudioLoaded() == true) ? juce::Logger::outputDebugString("paramsView.getAudioLoaded() true") : juce::Logger::outputDebugString("paramsView.getAudioLoaded() false");
    //(paramsView.getAudioBuffer() == nullptr) ? juce::Logger::outputDebugString("paramsView.getAudioBuffer() nullptr") : juce::Logger::outputDebugString("paramsView.getAudioBuffer() not nullptr");
}

void AudioFileFrame::filesDropped(const juce::StringArray& files, int x, int y)
{
    play_btn.setEnabled(false);
    paramsView.setIsPlaying(false);
    for(juce::String file : files)
    {
        if(isInterestedInFileDrag(file))
        {
            audioProcessor.loadFile(file);
        }
    }
}

void AudioFileFrame::stopFileButtonClicked() { paramsView.setIsPlaying(false); }

void AudioFileFrame::playFileButtonClicked()
{
    if(paramsView.getIsPlaying() == true)
    {
        paramsView.setIsPlaying(false);
        setPlayButtonImagePause();
    }
    else
    {
        paramsView.setIsPlaying(true);
        setPlayButtonImagePlay();
    }
}

void AudioFileFrame::setPlayButtonImagePlay()
{
    play_btn.setImages(juce::Drawable::createFromImageData(BinaryData::Play_svg, BinaryData::Play_svgSize).get(),
        juce::Drawable::createFromImageData(BinaryData::Play_Fill_svg, BinaryData::Play_Fill_svgSize).get(), nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr);
}

void AudioFileFrame::setPlayButtonImagePause()
{
    paramsView.setIsPlaying(false);
    play_btn.setImages(juce::Drawable::createFromImageData(BinaryData::Pause_svg, BinaryData::Pause_svgSize).get(),
        juce::Drawable::createFromImageData(BinaryData::Pause_Fill_svg, BinaryData::Pause_Fill_svgSize).get(), nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr);
}

void AudioFileFrame::setOpenButtonImageOpen()
{
    open_btn.setImages(juce::Drawable::createFromImageData(BinaryData::AddFolder_svg, BinaryData::AddFolder_svgSize).get(),
        juce::Drawable::createFromImageData(BinaryData::AddFolder_Fill_svg, BinaryData::AddFolder_Fill_svgSize).get(), nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr);
}

void AudioFileFrame::setStopButtonImageStop()
{
    stop_btn.setImages(juce::Drawable::createFromImageData(
        BinaryData::Stop_svg, BinaryData::Stop_svgSize).get(),
        juce::Drawable::createFromImageData(
        BinaryData::Stop_Fill_svg, BinaryData::Stop_Fill_svgSize).get(), 
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

bool AudioFileFrame::isInterestedInFileDrag(const juce::StringArray& files)
{
    // is it an audio file ?
    for(juce::String file : files)
    {
        if(file.endsWithIgnoreCase(".wav") || file.endsWithIgnoreCase(".aif") || file.endsWithIgnoreCase(".mp3"))
        {
            return true;
        }
    }
    return false;
}

// component section

void AudioFileFrame::paint(juce::Graphics& g) { g.fillAll(MyColours::brightBlue); }

void AudioFileFrame::resized()
{
    juce::Rectangle<int> localArea = getLocalBounds();
    float w = getWidth() / 30.f;
    float h = getHeight() / 30.f;

    localArea.removeFromTop(static_cast<int>(h));
    localArea.removeFromBottom(static_cast<int>(h));

    juce::Rectangle<int> buttonsArea = localArea.removeFromLeft(static_cast<int>(w));
    localArea.removeFromLeft(static_cast<int>(h));
    localArea.removeFromRight(static_cast<int>(h));

    juce::Rectangle<int> SpectrumArea = localArea.removeFromLeft(localArea.getWidth());

    // on d	clare les flexbox
    juce::FlexBox flexboxMain;
    flexboxMain.flexDirection = juce::FlexBox::Direction::row;

    juce::FlexBox flexboxLeft;
    flexboxLeft.flexDirection = juce::FlexBox::Direction::column;
    flexboxLeft.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxLeft.alignContent = juce::FlexBox::AlignContent::stretch;
    flexboxLeft.alignItems = juce::FlexBox::AlignItems::stretch;
    flexboxLeft.justifyContent = juce::FlexBox::JustifyContent::center;

    juce::FlexBox flexboxRight;
    flexboxRight.flexDirection = juce::FlexBox::Direction::column;
    flexboxRight.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexboxRight.alignContent = juce::FlexBox::AlignContent::stretch;
    flexboxRight.alignItems = juce::FlexBox::AlignItems::stretch;

    // on ajoute les items dans les flexbox

    flexboxLeft.items.add(juce::FlexItem(open_btn).withHeight(buttonsArea.getWidth() * 2.f));
    flexboxLeft.items.add(juce::FlexItem(play_btn).withHeight(buttonsArea.getWidth() * 1.33f));

    flexboxRight.items.add(juce::FlexItem(thumbnailComponent).withHeight(SpectrumArea.getHeight() * 1.f));

    flexboxMain.items.add(juce::FlexItem(flexboxLeft).withFlex(0.05f).withMargin(h));
    flexboxMain.items.add(juce::FlexItem(flexboxRight).withFlex(0.95f).withMargin(h));
    flexboxMain.performLayout(getLocalBounds().toFloat());
}

void AudioFileFrame::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if(source == &audioProcessor)
    {
        const juce::File& f = audioProcessor.getCurrentFile();
        bool valid = f.existsAsFile();
        if(valid && paramsView.getSampleSource())
            thumbnailComponent.setFile(f);
        play_btn.setEnabled(valid);
    }
}
