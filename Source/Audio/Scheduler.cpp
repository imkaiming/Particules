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
	nextOnset = 1;
	nbActiveGrains = 0;
	numChannels = 0;
	stateParams->setGrains(&grains);
}

Scheduler::~Scheduler()
{
	freeActiveGrains();
	stateParams = nullptr;
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
	nextOnset = 1;
	nbActiveGrains = 0;

	phaseMod.reset();
	phaseMod.setSampleRate(stateParams->getSampleRate());
	phaseMod.setMod(stateParams->getTraversalModeValue());
	phaseMod.setFrequency(1 / stateParams->getTraversalTimeValue());

}

Grain* Scheduler::generateGrain(int numSamples)
{
	// on récupère la valeur en samples par rapport au pourcentage de la position dans le fichier audio

	int durationSamples = static_cast<int>(stateParams->getDuration() * stateParams->getSampleRate());
	int widthSamples = static_cast<int>(durationSamples * stateParams->getEnvWidth());
	int positionSamples = static_cast<int>(stateParams->getNumSamples() * stateParams->getFilePosition());
	int selectionSamples = static_cast<int>(stateParams->getNumSamples() * stateParams->getWindowSelection());


	phaseMod.setFrequency(1 / stateParams->getTraversalTimeValue());
	phaseMod.setMod(stateParams->getTraversalModeValue());
	positionSamples += phaseMod.getValue() * selectionSamples;

	//DBG("value : " << phaseMod.getValue());
	//DBG("selectionSamples : " << selectionSamples);
	//DBG("positionSamples : " << positionSamples);

	return new Grain(
		durationSamples, // le nombre total de sample dans le grain
		numChannels,
		stateParams->getEnvelopeType(),
		stateParams->getSpeed(),
		widthSamples, // le nombre de sample qu'il y a entre la fin du fade in et le debut du fade out
		positionSamples, // le nombre de sample qui determine la position dans le fichier pour le depart
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
			for (size_t channel = 0; channel < numChannels; ++channel)
			{
				// add rms here + amplitude to the grains
				float* blockPointer = audioBlock->getChannelPointer(channel);
				blockPointer[sample] += grain->getCurrentSample(channel); // add rms here
			}

			grain->update();

			if (!grain->isActive())
			{
				grains.remove(grains.indexOf(grain));
				delete grain;
				--nbActiveGrains;

				if (grains.isEmpty())
					stateParams->setIsGrainsEmpty(true);


			}
		}

	}

	if (stateParams->getIsPlaying() == true) {
		if (--nextOnset == 0) // on avance à chaque sample
		{
			// TODO : récupérer les valeur random du stateParam pour les donner au grain avant de le générer.

			Grain* unGrain = generateGrain(numSamples);
			++nbActiveGrains;

			stateParams->setIsGrainsEmpty(false);


			grains.add(unGrain);
			int interOnset = stateParams->getInterOnset(); // ajouter le random ici
			nextOnset += interOnset; // determine le moment où prochain grain sera créer
		}
	}
	//else if (grains.isEmpty()) { // ça sonne moins réactif
	//	nextOnset = 1;
	//}
	else {
		nextOnset = 1;
	}

	phaseMod.advance();

	// pour le controle de la sommation (on ne veut pas de division par zero)
	//float weight = 1.0f / static_cast<float>(nbActiveGrains + 1);

	//float linearCoef = juce::Decibels::decibelsToGain(-3.f); // logarithmique... max 1 - min 0.7
	// 0.707 = 10 ^ (-3 / 10)
					// on vérifie si il reste encore des grains sinon on met à jours le verrou de play

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


// on connecte le scheduler avec le grainVisualizer qui se charge 
// d'envoyer les mises à jours des positions des grains.
//void Scheduler::setGrainVisualizer()
//{
//	this->grainVisualizer =  stateParams->getGrainVisualizer();
//	grainVisualizer->setGrains(&grains);
//}