#include "UIState.h"
#include "../../utils/struct/VisualSnapshot.h"

namespace particules
{
    UIState::UIState() : formatManager{}, numSamples{0}, cache{10}, audioThumbnail{samplesPerThumbnail, formatManager, cache}
    {
        formatManager.registerBasicFormats();
    }
    void UIState::setSource(const juce::File& f) noexcept
    {
        audioThumbnail.setSource(new juce::FileInputSource(f));
        setFileLoaded(true);
    }

    void UIState::setFileLoaded(bool b)
    {
        fileLoaded.store(b);
        sendChangeMessage(); // "a file is loaded you can now enable new logic"
    }

    const VisualSnapshot& UIState::getSnapshot() const noexcept { return visualBuffer->getSnapshot(); }
}