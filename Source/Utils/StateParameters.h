#pragma once

#include <JuceHeader.h>
#include <atomic>

#include "ParamsID.h"
#include "../Audio/Grain.h"
#include "../Utils/GrainVisualizer.h"
#include "../Framework/Core.h"

//using ValueTreeState = juce::AudioProcessorValueTreeState;

// TODO@ renommer en RuntimeParameters

struct GranularParamsView
{
	const std::atomic<float>* mix = nullptr;
	const std::atomic<float>* gain = nullptr;
	const std::atomic<float>* density = nullptr;
	const std::atomic<float>* duration = nullptr;
	const std::atomic<float>* position = nullptr;
	const std::atomic<float>* selection = nullptr;
	const std::atomic<float>* speed = nullptr;
	const std::atomic<float>* envType = nullptr;
	const std::atomic<float>* traversalMode = nullptr;
	const std::atomic<float>* traversalTime = nullptr;

	const std::atomic<bool>* isPlaying = nullptr;
	const std::atomic<bool>* audioLoaded = nullptr;
};

class StateParameters
{
public:
	StateParameters();
	~StateParameters();

	void init(ValueTreeState& apvts, int numChannels, double sampleRate);

	// APVTS param reads
	//void setMix(std::atomic<float>* ptr) noexcept { mMix = ptr; };
	float getMix() const noexcept { return mMix ? convertToPercentage(mMix->load(std::memory_order_relaxed)) : 0.0f; }

	//void setGain(std::atomic<float>* ptr) noexcept { mGain = ptr; };
	float getGain() const noexcept { return mGain ? mGain->load(std::memory_order_relaxed) : 0.0f; }
	float getGainToDecibel() const noexcept { return mGain ? juce::Decibels::gainToDecibels(mGain->load(std::memory_order_relaxed)) : 0.0f; }

	//void setSpeed(std::atomic<float>* ptr) noexcept { mSpeed = ptr; };
	float getSpeed() const noexcept { return mSpeed ? mSpeed->load(std::memory_order_relaxed) : 0.0f; }

	//void setDuration(std::atomic<float>* ptr) noexcept { mDuration = ptr; };
	float getDuration() const noexcept { return mDuration ? mDuration->load(std::memory_order_relaxed) : 0.0f; }

	//void setDensity(std::atomic<float>* ptr)noexcept { mDensity = ptr; };
	float getDensity() const noexcept { return mDensity ? mDensity->load(std::memory_order_relaxed) : 0.0f; }

	//void setWindowSelection(std::atomic<float>* ptr)noexcept { mWindowSelection = ptr; };
	float getWindowSelection() const noexcept { return mWindowSelection ? mWindowSelection->load(std::memory_order_relaxed) : 0.0f; }

	//void setFilePosition(std::atomic<float>* ptr) noexcept { mFilePosition = ptr; };
	float getFilePosition() const noexcept { return mFilePosition ? mFilePosition->load(std::memory_order_relaxed) : 0.0f; }

	//void setEnvWidth(std::atomic<float>* ptr)noexcept { mEnvWidth = ptr; };
	float getEnvWidth() const noexcept { return mEnvWidth ? mEnvWidth->load(std::memory_order_relaxed) : 0.0f; }

	//void setTraversalTime(std::atomic<float>* ptr)noexcept { mTraversalTime = ptr; };
	float getTraversalTime() const noexcept { return mTraversalTime ? mTraversalTime->load(std::memory_order_relaxed) : 0.0f; }

	//void setEnvelopeType(std::atomic<float>* ptr)noexcept { mEnvelopeType = ptr; };
	int getEnvelopeType() const noexcept { return mEnvelopeType ? static_cast<int>(mEnvelopeType->load(std::memory_order_relaxed)) : 0; }

	//void setTraversalMode(std::atomic<float>* ptr) noexcept { mTraversalMode = ptr; };
	int getTraversalMode() const noexcept { return mTraversalMode ? static_cast<int>(mTraversalMode->load(std::memory_order_relaxed)) : 0; }

	// runtime flags (thread-safe)
	void setIsPlaying(bool b) noexcept { mIsPlaying.store(b, std::memory_order_relaxed); }
	void setIsAudioLoaded(bool b) noexcept { mIsAudioLoaded.store(b, std::memory_order_relaxed); }
	void setIsGrainsEmpty(bool b) noexcept { mIsGrainsEmpty.store(b, std::memory_order_relaxed); }

	bool getIsPlaying() const noexcept { return mIsPlaying.load(std::memory_order_relaxed); }
	bool getAudioLoaded() const noexcept { return mIsAudioLoaded.load(std::memory_order_relaxed); }
	bool getIsGrainsEmpty() const noexcept { return mIsGrainsEmpty.load(std::memory_order_relaxed); }


	// misc runtime
	void setNumSamples(int samples) noexcept { mNumSamples.store(samples, std::memory_order_relaxed); }
	int getNumSamples() const noexcept { return mNumSamples.load(std::memory_order_relaxed); }

	void setSampleRate(double sr) noexcept { mSampleRate.store(sr, std::memory_order_relaxed); }
	double getSampleRate() const noexcept { return mSampleRate.load(std::memory_order_relaxed); }

	int getNumChannels() const noexcept { return mNumChannels; }

	void setAudioBuffer(juce::AudioBuffer<float>* buffer) noexcept { mAudioFileBuffer = buffer; }
	juce::AudioBuffer<float>* getAudioBuffer() const noexcept { return mAudioFileBuffer; }

	void setGrains(juce::Array<Grain*>* grains) noexcept { mGrains = grains; }
	juce::Array<Grain*>* getGrains() const noexcept { return mGrains; }

	void setGrainVisualizer(GrainVisualizer* gv) noexcept { mGrainVisualizer = gv; }
	GrainVisualizer* getGrainVisualizer() const noexcept { return mGrainVisualizer; }

	void setInterOnSet();
	int getInterOnSet() const noexcept { return mInterOnset.load(std::memory_order_relaxed); };

	GranularParamsView makeView() noexcept
	{
		GranularParamsView v;
		v.density = mDensity;
		v.duration = mDuration;
		v.position = mFilePosition;
		v.selection = mWindowSelection;
		v.speed = mSpeed;
		v.mix = mMix;
		v.gain = mGain;
		v.envType = mEnvelopeType;
		v.traversalMode = mTraversalMode;
		v.traversalTime = mTraversalTime;
		v.isPlaying = &mIsPlaying;
		v.audioLoaded = &mIsAudioLoaded;
		return v;
	}

private:
	// APVTS values
	std::atomic<float>* mMix = nullptr;
	std::atomic<float>* mGain = nullptr;
	std::atomic<float>* mDensity = nullptr;
	std::atomic<float>* mDuration = nullptr;
	std::atomic<float>* mSpeed = nullptr;

	std::atomic<float>* mFilePosition = nullptr;
	std::atomic<float>* mWindowSelection = nullptr;
	std::atomic<float>* mEnvWidth = nullptr;

	std::atomic<float>* mEnvelopeType = nullptr;
	std::atomic<float>* mTraversalMode = nullptr;
	std::atomic<float>* mTraversalTime = nullptr;

	// runtime flags
	std::atomic<bool> mIsPlaying;
	std::atomic<bool> mIsAudioLoaded;
	std::atomic<bool> mIsGrainsEmpty;

	std::atomic<int> mNumSamples;
	std::atomic<double> mSampleRate;
	std::atomic<int> mNumChannels;
	std::atomic<int> mInterOnset;

	// non-atomic pointers: only safe if not mutated concurrently
	juce::AudioBuffer<float>* mAudioFileBuffer = nullptr;
	juce::Array<Grain*>* mGrains = nullptr;
	GrainVisualizer* mGrainVisualizer = nullptr;
};