#include "UIState.h"

#include "../../utils/struct/VisualSnapshot.h"
#include "../bridge/GrainVisualBuffer.h"

namespace particules
{
    UIState::UIState()
        : formatManager{}, fileLoaded{false}, numSamples{0}, cache{10},
          visualBuffer{nullptr}, audioThumbnail{samplesPerThumbnail, formatManager, cache} /*, visualBuffer{vb}*/
    {
        formatManager.registerBasicFormats();
    }
    void UIState::setSource(const juce::File& f) noexcept
    {
        //audioThumbnail.setSource(std::make_unique<juce::FileInputSource>(f));
        audioThumbnail.setSource(new juce::FileInputSource(f));
        currentFile = f;
        setFileLoaded(true);
    }

    void UIState::setFileLoaded(bool b)
    {
        fileLoaded.store(b, std::memory_order_relaxed);
        sendChangeMessage(); // "a file is loaded you can now enable new logic"
    }

    const VisualSnapshot& UIState::getSnapshot() const noexcept { return visualBuffer->getSnapshot(); }

    void UIState::init(const GrainVisualBuffer* vb) noexcept { visualBuffer = vb; }
}