#include <atomic>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    class VerticalMeter : public juce::Component, private juce::Timer
    {
    public:
        VerticalMeter()
        {
            startTimerHz(30);
            peakHold.store(-60.0f);
            currentLevel.store(-60.0f);
        }

        ~VerticalMeter() override = default;

        void pushLevel(float levelInDB)
        {
            // On ne fait qu'un store atomique, rien d'autre sur l'audio thread
            incomingLevel.store(levelInDB, std::memory_order_relaxed);
        }

        void paint(juce::Graphics& g) override
        {
            auto bounds = getLocalBounds().reduced(4);
            int w = bounds.getWidth();
            int h = bounds.getHeight();

            // Fond du mètre
            g.setColour(juce::Colour(0xFF1A1A1A));
            g.fillRoundedRectangle(bounds.toFloat(), 6.0f);

            // Zone des LEDs (on garde de l'espace en haut pour le peak et en bas pour le label)
            auto ledArea = bounds.reduced(4, 25);
            int ledW = ledArea.getWidth();
            int ledH = ledArea.getHeight();

            // Nombre de LEDs selon la hauteur disponible
            const int numLEDs = 32;
            const float spacing = 2.0f;
            const float ledHeight = (ledH - (numLEDs - 1) * spacing) / numLEDs;
            const float ledWidth = ledW * 0.6f; // 60% de la largeur, centré

            float x = ledArea.getCentreX() - ledWidth / 2;
            float y = ledArea.getY() + ledH; // On part du bas et on remonte

            float level = currentLevel.load();
            float peak = peakHold.load();

            for(int i = 0; i < numLEDs; ++i)
            {
                // Position Y (du bas vers le haut)
                float ledY = y - (i + 1) * (ledHeight + spacing);

                // Seuil de cette LED (de -60dB à +6dB par exemple)
                float thresholdDB = juce::jmap((float)i, 0.0f, (float)(numLEDs - 1), -60.0f, 6.0f);

                // Couleur selon le niveau
                juce::Colour ledColour;
                bool isLit = level >= thresholdDB;
                bool isPeak = peak >= thresholdDB && peakHoldCounter > 0;

                if(isPeak && i > numLEDs - 3) // Peak hold sur les 2 derniers = rouge/orange
                {
                    ledColour = juce::Colour(0xFFFF6B35); // Orange peak
                }
                else if(isLit)
                {
                    // Gradient simple : blanc -> blanc chaud
                    if(i > numLEDs - 4) // Top = warning zone
                        ledColour = juce::Colour(0xFFFFD700).brighter(0.2f); // Or
                    else
                        ledColour = juce::Colours::white.withAlpha(0.9f);
                }
                else
                {
                    // LED éteinte (gris foncé)
                    ledColour = juce::Colour(0xFF333333);
                }

                g.setColour(ledColour);

                // LED ronde (ellipse)
                g.fillEllipse(x, ledY, ledWidth, ledHeight);

                // Petit reflet sur les LED allumées
                if(isLit || (isPeak && i > numLEDs - 3))
                {
                    g.setColour(juce::Colours::white.withAlpha(0.4f));
                    g.fillEllipse(x + ledWidth * 0.2f, ledY + ledHeight * 0.1f, ledWidth * 0.3f, ledHeight * 0.3f);
                }
            }

            // Dessiner l'icône "OUT" / Haut-parleur en bas
            drawSpeakerIcon(g, bounds.removeFromBottom(20).reduced(2));
        }

        void drawSpeakerIcon(juce::Graphics& g, juce::Rectangle<int> area)
        {
            auto cx = area.getCentreX();
            auto cy = area.getCentreY();
            auto w = area.getWidth() * 0.8f;
            auto h = area.getHeight() * 0.8f;

            g.setColour(juce::Colours::grey.withAlpha(0.6f));

            // Forme de haut-parleur simplifiée (trapèze + triangle)
            juce::Path speaker;
            float x1 = cx - w * 0.3f;
            float x2 = cx - w * 0.1f;
            float x3 = cx + w * 0.4f;
            float y1 = cy - h * 0.4f;
            float y2 = cy + h * 0.4f;

            speaker.addTriangle(x2, y1, x2, y2, x3, cy); // Partie avant (cone)
            speaker.addRectangle(x1, y1 + h * 0.2f, w * 0.15f, h * 0.6f); // Arrière (aimant)

            g.fillPath(speaker);

            // Texte "OUT" sous l'icône ou à côté selon l'espace
            g.setFont(juce::Font(10.0f).withTypefaceStyle("Bold"));
            g.drawText("OUT", area.removeFromBottom(10), juce::Justification::centred);
        }

        void timerCallback() override
        {
            // Transférer depuis l'audio thread vers le message thread
            const float incoming = currentLevel.load(std::memory_order_relaxed);

            if(incoming > displayLevel)
                displayLevel += (incoming - displayLevel) * 0.8f; // attack ~50ms
            else
                displayLevel += (incoming - displayLevel) * 0.15f; // release ~200ms

            // Peak hold : monter instantanément
            if(incoming > displayPeak)
            {
                displayPeak = incoming;
                peakHoldFrames = 20; // ~660ms à 30Hz
            }
            // Decay du peak : -12 dB/s fixe
            else if(peakHoldFrames > 0)
            {
                peakHoldFrames--;
            }
            else
            {
                displayPeak -= 12.0f / 30.0f; // -12dB par seconde à 30Hz
                displayPeak = juce::jmax(displayPeak, displayLevel, -60.0f);
            }

            repaint();
        }

    private:
        std::atomic<float> currentLevel;
        std::atomic<float> peakHold;
        int peakHoldCounter = 0;

        // Atomique : seul endroit lu par l'audio thread
        std::atomic<float> incomingLevel{-60.0f};

        // Variables du message thread uniquement — pas besoin d'atomic
        float displayLevel = -60.0f;
        float displayPeak = -60.0f;
        int peakHoldFrames = 0;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticalMeter)
    };
}