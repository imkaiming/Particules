/*
  ==============================================================================

	Scheduler.cpp
	Created: 15 Feb 2023 1:44:18pm
	Author:  user

  ==============================================================================
*/

#include "Scheduler.h"

Scheduler::Scheduler(StateParameters* stateParams) : stateParams(stateParams)
{

}

Scheduler::~Scheduler()
{
	freeActiveGrains();
}

void Scheduler::freeActiveGrains()
{
	for (int i = 0; i < this->grains.size(); ++i)
	{
		Grain* grain = this->grains.removeAndReturn(i);
		delete grain;
	}
}

// restore the default value
void Scheduler::init(int numChannels)
{
	freeActiveGrains();
	this->numChannels = numChannels;
	//interOnset = 0;
	nextOnset = 1;
	nbActiveGrains = 0;
}


//void Scheduler::generateGrain()
//{
//	grains.add(
//		new Grain(
//			round(stateParams->getDuration() / stateParams->getDensity() * stateParams->getSampleRate()),
//			numChannels,
//			stateParams->getEnvelopeType())
//	);
//	++nbActiveGrains;
//	//juce::Logger::outputDebugString("creation d'un grain : " 
//	//	+ (juce::String)nbActiveGrains + " density : " + (juce::String)stateParams->getDensity() + " duration : "
//	//	+ (juce::String)stateParams->getDuration() + " interonset : " + (juce::String)stateParams->getInterOnset());
//
//}

// Realtime distribution of grains must be activated in timesequential order.
// Non realtime distribution ofgrains must be activated in random order according to the required density (nextOnset).
// generate one active grain at a time and set the inter-onset value for the next grain
void Scheduler::synthesize(AudioBlock* audioBlock, int sample, int numSamples) // , juce::AudioBuffer<float>* grainBuffer)
{
	// si on a aucun grain alors on écrit rien dans le buffer
	if (grains.isEmpty()) {
		for (size_t channel = 0; channel < numChannels; channel++) {
			audioBlock->addSample(channel, sample, 0.f);
		}
	}
	else {
		for (Grain* grain : grains)
		{
			if (sample == 0) {
				grain->updateBuffer(audioBlock);
			}

			for (size_t channel = 0; channel < numChannels; ++channel)
			{
				// gérer le pan ici 

				float* blockPointer = audioBlock->getChannelPointer(channel);
				blockPointer[sample] += grain->getCurrentSample(channel); // add rms here

				//juce::Logger::outputDebugString("channel : " + (juce::String)channel);
				//juce::Logger::outputDebugString("sample : " + (juce::String)sample);

				//audioBlock->addSample(channel, sample, grain->getCurrentSample(channel));

			}

			grain->update();


			if (!grain->isActive())
			{
				grains.remove(grains.indexOf(grain));
				delete grain;
				--nbActiveGrains;
				//juce::Logger::outputDebugString("suppression d'un grain");

			}
		}

	}

	if (--nextOnset == 0) // on avance à chaque sample
	{
		// TODO : récupérer les valeur random du stateParam pour les donner au grain avant de le générer.
		Grain* unGrain = new Grain(
			round(stateParams->getDuration() / stateParams->getDensity() * stateParams->getSampleRate()),
			numChannels,
			stateParams->getEnvelopeType(),
			stateParams->getSpeed(),
			audioBlock
		);
		++nbActiveGrains;
		//generateGrain();

		if (!grains.isEmpty())
		{
			// on veut le crossfade du dernier grain avant 
			// pour synchroniser les rampes du dernier grains et de celui qu'on va ajouter
			int crossfade = grains.getLast()->remainingLife();
			unGrain->applyCrossFade(crossfade, true);
			grains.getLast()->applyCrossFade(crossfade, false);
		}


		grains.add(unGrain);
		int interOnset = stateParams->getInterOnset(); // ajouter le random ici
		nextOnset += interOnset; // determine le moment où prochain grain sera créer
	}


	// pour le controle de la sommation (on ne veut pas de division par zero)
	//float weight = 1.0f / static_cast<float>(nbActiveGrains + 1);

	//float linearCoef = juce::Decibels::decibelsToGain(-3.f); // logarithmique... max 1 - min 0.7
	// 0.707 = 10 ^ (-3 / 10)

	
}

//float getWeight(const juce::AudioBuffer<float>& grain)
//{
//
//	float rms = 0.0f;
//	for (int ch = 0; ch < grain.getNumChannels(); ++ch)
//	{
//		const float* channelData = grain.getReadPointer(ch);
//		for (int i = 0; i < grain.getNumSamples(); ++i)
//		{
//			rms += channelData[i] * channelData[i];
//		}
//	}
//	rms = std::sqrt(rms / (grain.getNumChannels() * grain.getNumSamples()));
//
//	// Calculate the gain factor to prevent saturation
//	const float maxGain = 1.0f / std::sqrt(2.0f);
//	const float targetRMS = 0.707f; // Target RMS value for the overall mix
//	const float gain = std::min(maxGain, targetRMS / rms);
//
//	// Calculate the weight as the reciprocal of the gain factor
//	const float weight = 1.0f / gain;
//
//	return weight;
//}
