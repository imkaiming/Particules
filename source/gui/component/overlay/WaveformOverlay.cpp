#include "WaveformOverlay.h"
#include "../../lookandfeelv2/Colours.h"
#include <juce_core/juce_core.h>

namespace particules
{
    WaveformOverlay::WaveformOverlay(
        juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& posId, const juce::ParameterID& spanId)
        : dragStartX{0.f}, dragStartPos{0.f}, dragStartSpan{0.f}, isAudioLoaded{false}, currentHover{HoverState::None}, dragMode{DragMode::None}
    {
        rawPosValue = apvts.getRawParameterValue(posId.getParamID());
        rawSpanValue = apvts.getRawParameterValue(spanId.getParamID());

        positionAttachment =
            std::make_unique<juce::ParameterAttachment>(*apvts.getParameter(posId.getParamID()), [this](float) { repaint(); });

        spanAttachment =
            std::make_unique<juce::ParameterAttachment>(*apvts.getParameter(spanId.getParamID()), [this](float) { repaint(); });

        setInterceptsMouseClicks(false, false);
    }

    void WaveformOverlay::setAudioLoaded(bool loaded)
    {
        isAudioLoaded = loaded;
        setInterceptsMouseClicks(loaded, loaded);
        repaint();
    }

    void WaveformOverlay::paint(juce::Graphics& g)
    {
        if(!isAudioLoaded)
            return;

        // Read directly from APVTS
        const float pos = rawPosValue->load();
        const float span = rawSpanValue->load();

        const float w = static_cast<float>(getWidth());
        const float h = static_cast<float>(getHeight());

        const float posPx = pos * w;
        const float spanPx = span * w;
        const float spanEndPx = posPx + spanPx;

        const bool isPosHovered = (currentHover == HoverState::Position);
        const bool isBodySpanHovered = (currentHover == HoverState::Body );
        const bool isEdgeHovered = (currentHover == HoverState::Edge);

        // 1. SPAN REGION
        juce::Colour spanColor = colours::lavender.withAlpha(0.2f);
        if(isBodySpanHovered)
            spanColor = spanColor.brighter(0.2f);

        g.setColour(spanColor);
        if(spanEndPx > w)
        {
            g.fillRect(posPx, 0.0f, w - posPx, h);
            g.fillRect(0.0f, 0.0f, spanEndPx - w, h);
        }
        else if(spanPx > 0.0f)
        {
            g.fillRect(posPx, 0.0f, spanPx, h);
        }

        // 2. POSITION LINE
        juce::Colour lineColor = coloursv2::perleBlanc.withAlpha(0.55f);
        if(isPosHovered)
            lineColor = lineColor.withAlpha(0.75f);

        g.setColour(lineColor);
        g.drawLine(posPx, 0.0f, posPx, h, 2.0f);

        // 3. HANDLES
        const float handleSize = 10.0f;
        const float half = handleSize * 0.5f;

        // Position Handle
        juce::Colour posHandleColor = lineColor;
        if(isPosHovered)
            posHandleColor = lineColor;

        g.setColour(posHandleColor);
        g.fillRoundedRectangle(posPx - half, 0.0f, handleSize, handleSize, 2.0f);

        // Span Handle : Right Edge
        float spanHandleX = (spanEndPx > w) ? (spanEndPx - w) : spanEndPx;
        juce::Colour spanHandleColor = juce::Colours::red.brighter(0.5f).withAlpha(0.75f);
        if(isEdgeHovered)
            spanHandleColor = spanHandleColor.brighter(0.2f); 

        g.setColour(spanHandleColor);
        g.fillRoundedRectangle(spanHandleX - half, (h * 0.5f) - half, handleSize, handleSize, 2.0f);
    }

    void WaveformOverlay::mouseDown(const juce::MouseEvent& e)
    {
        if(!isAudioLoaded)
            return;

        currentHover = getHoverStateAt(e.position.x);

        dragStartX = e.position.x;
        dragStartPos = rawPosValue->load();
        dragStartSpan = rawSpanValue->load();

        if(currentHover == HoverState::Edge)
        {
            dragMode = DragMode::SpanEdge;
            spanAttachment->beginGesture();
        }
        else if(currentHover == HoverState::Body)
        {
            dragMode = DragMode::SpanBody;
            positionAttachment->beginGesture();
        }
        else if(currentHover == HoverState::Position)
        {
            dragMode = DragMode::Position;
            positionAttachment->beginGesture();
        }
    }

    void WaveformOverlay::mouseDrag(const juce::MouseEvent& e)
    {
        if(dragMode == DragMode::None)
            return;

        const float w = static_cast<float>(getWidth());
        float deltaNorm = (e.position.x - dragStartX) / w;

        if(dragMode == DragMode::Position)
        {
            float newPos = juce::jlimit(0.0f, 1.0f, dragStartPos + deltaNorm);
            positionAttachment->setValueAsPartOfGesture(newPos);
        }
        else if(dragMode == DragMode::SpanBody)
        {
            // span should move with the position but keep its value
            float newPos = juce::jlimit(0.0f, 1.0f, dragStartPos + deltaNorm);
            positionAttachment->setValueAsPartOfGesture(newPos);
        }
        else if(dragMode == DragMode::SpanEdge)
        {
            // position shouldnt move while moving the span edge
            float currentEndNorm = (dragStartPos + dragStartSpan) + deltaNorm;
            float newSpan = juce::jmax(0.0f, currentEndNorm - dragStartPos);
            spanAttachment->setValueAsPartOfGesture(newSpan);
        }
    }

    void WaveformOverlay::mouseUp(const juce::MouseEvent&)
    {
        if(dragMode == DragMode::Position || dragMode == DragMode::SpanBody)
            positionAttachment->endGesture();

        if(dragMode == DragMode::SpanEdge)
            spanAttachment->endGesture();

        dragMode = DragMode::None;
    }

    void WaveformOverlay::mouseMove(const juce::MouseEvent& e)
    {
        HoverState newState = getHoverStateAt(e.position.x);

        if(newState != currentHover)
        {
            currentHover = newState;
            switch(newState)
            {
                case HoverState::Position:
                case HoverState::Edge:
                    setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
                    break;
                case HoverState::Body:
                    setMouseCursor(juce::MouseCursor::DraggingHandCursor); 
                    break;
                default:
                    setMouseCursor(juce::MouseCursor::NormalCursor);
                    break;
            }
            repaint();
        }
    }

    void WaveformOverlay::mouseExit(const juce::MouseEvent& /*e*/) 
    {
        if(currentHover != HoverState::None)
        {
            currentHover = HoverState::None;
            repaint();
        }
    }

    WaveformOverlay::HoverState WaveformOverlay::getHoverStateAt(float mouseX) const
    {
        if(!isAudioLoaded)
            return HoverState::None;

        const float pos = rawPosValue->load();
        const float span = rawSpanValue->load();
        const float w = static_cast<float>(getWidth());

        const float posPx = pos * w;
        const float spanEndPx = posPx + (span * w);
        const float wrappedSpanEnd = (spanEndPx > w) ? (spanEndPx - w) : spanEndPx;

        const float edgeTolerance = 6.0f;

        if(std::abs(mouseX - wrappedSpanEnd) < edgeTolerance)
            return HoverState::Edge;
        if(mouseX > posPx + edgeTolerance && mouseX < wrappedSpanEnd - edgeTolerance)
            return HoverState::Body;
        if(std::abs(mouseX - posPx) < edgeTolerance)
            return HoverState::Position;

        return HoverState::None;
    }

}