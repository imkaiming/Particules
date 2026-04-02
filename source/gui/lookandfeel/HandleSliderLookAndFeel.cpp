#include "HandleSliderLookAndFeel.h"
#include "BinaryData.h"

namespace particules
{
    HandleSliderLookAndFeel::HandleSliderLookAndFeel()
    {
        metalHandleImage = juce::ImageFileFormat::loadFrom(BinaryData::metal_handle_01_png, BinaryData::metal_handle_01_pngSize);
        metalHandleImage = metalHandleImage.convertedToFormat(juce::Image::ARGB);
    }

    void HandleSliderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider)
    {
        if(metalHandleImage.isNull())
            return;

        const bool hover = slider.isMouseOverOrDragging();
        const float cy = y + height * 0.5f;
        const float handleSize = hover ? 8.f : 7.0f;

        //float handleX = sliderPos - handleSize * 0.5f;
        //float handleY = y + (height * 0.5f) - handleSize * 0.5f;

        juce::Rectangle<float> handleBounds(sliderPos - handleSize, cy - handleSize, handleSize * 2.0f, handleSize * 2.0f);

        g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
        g.drawImage(metalHandleImage, handleBounds, juce::RectanglePlacement::stretchToFit);

        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.fillRect((float)x, cy - 1.0f, (float)width, 2.0f);

        //g.setColour(juce::Colours::white);
        //g.fillEllipse(sliderPos - handleSize, cy - handleSize, handleSize * 2.0f, handleSize * 2.0f);

        g.setColour(juce::Colours::black.withAlpha(0.4f));
        g.drawEllipse(sliderPos - handleSize, cy - handleSize, handleSize * 2.0f, handleSize * 2.0f, 1.0f);
    }

}
