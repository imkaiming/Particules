#pragma once

#include "GuiAppEditor.h"
#include "GuiAppProcessor.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class GuiApp : public juce::JUCEApplication
{
public:
    GuiApp() : processor{} {};

    const juce::String getApplicationName() override { return "ParticulesGUI"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String& commandLine) override
    {
        mainWindow = std::make_unique<MainWindow>("GUI", new particules::GuiAppEditor(processor));
    }

    void shutdown() override { mainWindow = nullptr; }

    void systemRequestedQuit() override { quit(); }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name, juce::Component* contentComponent)
            : DocumentWindow(name, juce::Colours::darkgrey, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(contentComponent, true);
            centreWithSize(1200, 800); // Taille par défaut
            setVisible(true);
        }

        void closeButtonPressed() override { JUCEApplication::getInstance()->systemRequestedQuit(); }
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
    particules::GuiAppProcessor processor;
};

START_JUCE_APPLICATION(GuiApp)