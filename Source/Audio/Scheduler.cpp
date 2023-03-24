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


Grain* Scheduler::generateGrain(int numSamples)
{
	// on récupère la valeur en samples par rapport au pourcentage de la position dans le fichier audio
	int durationSamples = static_cast<int>(stateParams->getDuration() * stateParams->getSampleRate());
	int widthSamples = static_cast<int>(durationSamples * stateParams->getEnvWidth());
	int positionSamples = static_cast<int>(numSamples * stateParams->getFilePosition());
	int selectionSamples = static_cast<int>(numSamples * stateParams->getWindowSelection());

	// Si selectionSamples est supérieur à numSamples, alors on loop le samples


	//if (duration / getSampleRate() > selectionSamples) ?
	//	if (positionSamples + selectionSamples > numSamples) ?
	//		1 soit on bloque les limites
	//		2 soit on retourne en arrière (reverse)
	//		2 soit on retourne au début  (modulo)
	//		3 soit on va au dela (libre)

	// définir les MODES de gestion des limites du buffer pour les grains ici
	// utiliser un switch case (1, 2, 3, 4)

	// 1. on ne veut pas que les samples aillent au delà des limites accordées par le buffer
	if (positionSamples + selectionSamples > numSamples)
	{
		selectionSamples = numSamples - positionSamples;
	}
	if (durationSamples > selectionSamples)
	{
		durationSamples = selectionSamples;
	}

	return new Grain(
		//round(stateParams->getDuration() / stateParams->getDensity() * stateParams->getSampleRate()),
		durationSamples,
		numChannels,
		stateParams->getEnvelopeType(),
		stateParams->getSpeed(),
		widthSamples,
		positionSamples,
		selectionSamples,
		stateParams->getAudioBuffer()
	);

}

// Realtime distribution of grains must be activated in timesequential order.
// Non realtime distribution ofgrains must be activated in random order according to the required density (nextOnset).
// generate one active grain at a time and set the inter-onset value for the next grain
void Scheduler::synthesize(AudioBlock* audioBlock, int sample, int numSamples)
{
	// si on a aucun grain alors on écrit du silence dans le buffer
	if (grains.isEmpty())
	{
		for (size_t channel = 0; channel < numChannels; channel++) {
			audioBlock->addSample(channel, sample, 0.f);
		}
	}
	else
	{
		for (Grain* grain : grains)
		{
			//if (sample == 0) {
			//	grain->updateBuffer(buffer);
			//}

			for (size_t channel = 0; channel < numChannels; ++channel)
			{
				// gérer le pan ici 
				//float* channelData = buffer->getWritePointer(channel);
				//channelData[sample] += grain->getCurrentSample(channel); // add rms here + amplitude to the grains
				float* blockPointer = audioBlock->getChannelPointer(channel);
				blockPointer[sample] += grain->getCurrentSample(channel); // add rms here
			}

			grain->update();

			if (!grain->isActive())
			{
				grains.remove(grains.indexOf(grain));
				delete grain;
				--nbActiveGrains;

			}
		}

	}

	if (--nextOnset == 0) // on avance à chaque sample
	{
		// TODO : récupérer les valeur random du stateParam pour les donner au grain avant de le générer.
		Grain* unGrain = generateGrain(numSamples);
		++nbActiveGrains;

		//if (!grains.isEmpty()) // if(nbActiveGrains != 0)
		//{
		//	// on veut le crossfade du dernier grain avant 
		//	// pour synchroniser les rampes du dernier grains et de celui qu'on va ajouter
		//	// TODO a remplacer part grain->applyEnvelope() avec un paramètre envelopeWidth 
		//	int crossfade = grains.getLast()->remainingLife();
		//	grains.getLast()->applyCrossFade(crossfade, false);
		//	unGrain->applyCrossFade(crossfade, true);

		//}


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
