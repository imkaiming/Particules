/*
  ==============================================================================

	Parameters.cpp
	Created: 26 Feb 2023 12:38:46am
	Author:  user

  ==============================================================================
*/


#include "StateParameters.h"

StateParameters::StateParameters(): mSampleRate{0.0}, mNumChannels{0}, mNumSamples{0}, mIsGrainsEmpty{true}, mIsPlaying{false}, mIsAudioLoaded{false}, mInterOnset{0}
{
}

StateParameters::~StateParameters()
{
	mMix = nullptr;
	mGain = nullptr;
	mDensity = nullptr;
	mDuration = nullptr;
	mSpeed = nullptr;
	mFilePosition = nullptr;
	mWindowSelection = nullptr;
	mEnvWidth = nullptr;
	mEnvelopeType = nullptr;
	mTraversalMode = nullptr;
	mTraversalTime = nullptr;

	mAudioFileBuffer = nullptr;
	mGrains = nullptr;
	mGrainVisualizer = nullptr;
}

void StateParameters::init(ValueTreeState& apvts, int numChannels, double sampleRate)
{
	mNumChannels.store(numChannels, std::memory_order_relaxed);
	mSampleRate.store(sampleRate, std::memory_order_relaxed);
	mMix = apvts.getRawParameterValue(Param::Mix::id);
	mGain = apvts.getRawParameterValue(Param::Gain::id);
	mDensity = apvts.getRawParameterValue(Param::Density::id);
	mDuration = apvts.getRawParameterValue(Param::Duration::id);
	mSpeed = apvts.getRawParameterValue(Param::Speed::id);
	mFilePosition = apvts.getRawParameterValue(Param::Position::id);
	mWindowSelection = apvts.getRawParameterValue(Param::Selection::id);
	mEnvelopeType = apvts.getRawParameterValue(Param::EnvelopeType::id);
	mEnvWidth = apvts.getRawParameterValue(Param::EnvelopeWidth::id);
	mTraversalMode = apvts.getRawParameterValue(Param::TraversalMode::id);
	mTraversalTime = apvts.getRawParameterValue(Param::TraversalTime::id);

}
// Le callback qui envoit newValue ne provient pas du apvts mais du slider associé au Listener
/*void StateParameters::parameterChanged(const juce::String& parameterID, float newValue)
{
	if (parameterID == MIX_ID) {
		mix = static_cast<int>(newValue / 100.f);
		juce::Logger::outputDebugString("StateParameters mix : " + (const juce::String)mix);
	}
	if (parameterID == GAIN_ID) {
		gain = pow(10, newValue / 20);
		juce::Logger::outputDebugString("StateParameters gain : " + (const juce::String)gain);
	}
	else if (parameterID == DENSITY_ID) {
		density = newValue;
		juce::Logger::outputDebugString("StateParameters density : " + (const juce::String)density);
		setInterOnSet();
	}
	else if (parameterID == DURATION_ID) {
		duration = newValue;
		juce::Logger::outputDebugString("StateParameters duration : " + (const juce::String)duration);
	}
	else if (parameterID == PITCH_ID) {
		pitch = static_cast<int>(newValue);
		juce::Logger::outputDebugString("StateParameters pitch : " + (const juce::String)pitch);
	}
}*/


void StateParameters::setInterOnSet()
{
	//mInterOnset->store(static_cast<int>(round(getSampleRate() / getDensity()), std::memory_order_release));
	const float density = getDensity();
	if(density <= 0.0) return;

	const int sr = getNumSamples();
	const int inter = static_cast<int>(std::lround(mSampleRate / density));
	mInterOnset.store(inter, std::memory_order_relaxed);


}




//void StateParameters::updateGrainVisualizer()
//{
//	grainVisualizer->update();
//}
