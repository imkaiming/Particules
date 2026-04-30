#include "SliderOnWaveform.h"

#include <juce_core/juce_core.h>

#include "gui/lookandfeelv2/Colours.h"

namespace particules
{
    SliderOnWaveform::SliderOnWaveform(ValueTreeState& apvts, const juce::ParameterID& posId, const juce::ParameterID& spanId)
        : dragStartX{0.f}, dragStartPos{0.f}, dragStartSpan{0.f}, isAudioLoaded{false}, currentHover{HoverState::None},
          dragMode{DragMode::None}, rawSpanValue{nullptr}, rawPosValue{nullptr}
    {
        rawPosValue = apvts.getRawParameterValue(posId.getParamID());
        rawSpanValue = apvts.getRawParameterValue(spanId.getParamID());

        positionAttachment =
            std::make_unique<juce::ParameterAttachment>(*apvts.getParameter(posId.getParamID()), [this](float) { repaint(); });

        spanAttachment =
            std::make_unique<juce::ParameterAttachment>(*apvts.getParameter(spanId.getParamID()), [this](float) { repaint(); });

        setInterceptsMouseClicks(false, false);
    }

    void SliderOnWaveform::setAudioLoaded(bool loaded)
    {
        isAudioLoaded = loaded;
        setInterceptsMouseClicks(loaded, loaded);
        repaint();
    }

    void SliderOnWaveform::paint(juce::Graphics& g)
    {
        if(!isAudioLoaded)
            return;

        const float pos = rawPosValue->load();
        const float span = rawSpanValue->load();

        const float w = static_cast<float>(getWidth());
        const float h = static_cast<float>(getHeight());

        const float posPx = pos * w;
        const float spanPx = span * w;
        const float spanEndPx = posPx + spanPx;

        const bool isPosHovered = (currentHover == HoverState::Position);
        const bool isBodySpanHovered = (currentHover == HoverState::Body);
        const bool isEdgeHovered = (currentHover == HoverState::Edge);

        // 1. span region
        color spanColor = colours::lavender.withAlpha(0.15f);
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

        // 2. position line
        color lineColor = coloursv2::perleBlanc.withAlpha(0.55f);
        if(isPosHovered)
            lineColor = lineColor.withAlpha(0.75f);

        g.setColour(lineColor);
        g.drawLine(posPx, 0.0f, posPx, h, 2.0f);

        // 3. handles
        const float handleSize = 10.0f;
        const float half = handleSize * 0.5f;

        // position handle
        color posHandleColor = lineColor;
        if(isPosHovered)
            posHandleColor = lineColor.withAlpha(0.75f);

        g.setColour(posHandleColor);
        g.fillRoundedRectangle(posPx - half, 0.0f, handleSize, handleSize, 2.0f);

        // Span Handle : Right Edge
        float spanHandleX = (spanEndPx > w) ? (spanEndPx - w) : spanEndPx;
        color spanHandleColor = juce::Colours::yellow.brighter(0.5f).withAlpha(0.75f);
        if(isEdgeHovered)
            spanHandleColor = spanHandleColor.brighter(0.2f);

        g.setColour(spanHandleColor);
        g.fillRoundedRectangle(spanHandleX - half, (h * 0.5f) - half, handleSize, handleSize, 2.0f);
    }

    void SliderOnWaveform::mouseDown(const juce::MouseEvent& e)
    {
        if(!isAudioLoaded)
            return;

        currentHover = getHoverStateAt(e.position.x);

        dragStartX = e.position.x;
        dragStartPos = rawPosValue->load();
        dragStartSpan = rawSpanValue->load();

        switch(currentHover)
        {
            case HoverState::Edge:
                dragMode = DragMode::SpanEdge;
                spanAttachment->beginGesture();
                break;

            case HoverState::Body:
                dragMode = DragMode::SpanBody;
                positionAttachment->beginGesture();
                break;

            case HoverState::Position:
                dragMode = DragMode::Position;
                positionAttachment->beginGesture();
                break;

            default:
                dragMode = DragMode::None;
                break;
        }
    }

    // convert pixel into normalized value
    void SliderOnWaveform::mouseDrag(const juce::MouseEvent& e)
    {
        if(dragMode == DragMode::None)
            return;

        const float w = static_cast<float>(getWidth());
        const float deltaNorm = (e.position.x - dragStartX) / w;

        if(dragMode == DragMode::Position)
        {
            float newPos = juce::jlimit(0.0f, 1.0f, dragStartPos + deltaNorm);
            positionAttachment->setValueAsPartOfGesture(newPos);
        }
        else if(dragMode == DragMode::SpanBody)
        {
            float newPos = juce::jlimit(0.0f, 1.0f, dragStartPos + deltaNorm);
            positionAttachment->setValueAsPartOfGesture(newPos);
        }
        else if(dragMode == DragMode::SpanEdge)
        {
            float currentEndNorm = (dragStartPos + dragStartSpan) + deltaNorm;
            float newSpan = juce::jmax(0.0f, currentEndNorm - dragStartPos);
            spanAttachment->setValueAsPartOfGesture(newSpan);
        }

        repaint();
    }

    void SliderOnWaveform::mouseUp(const juce::MouseEvent&)
    {
        if(dragMode == DragMode::Position || dragMode == DragMode::SpanBody)
            positionAttachment->endGesture();

        if(dragMode == DragMode::SpanEdge)
            spanAttachment->endGesture();

        dragMode = DragMode::None;
    }

    void SliderOnWaveform::mouseMove(const juce::MouseEvent& e)
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

    void SliderOnWaveform::mouseExit(const juce::MouseEvent& /*e*/)
    {
        if(currentHover != HoverState::None)
        {
            currentHover = HoverState::None;
            setMouseCursor(juce::MouseCursor::NormalCursor);
            repaint();
        }
    }

    SliderOnWaveform::HoverState SliderOnWaveform::getHoverStateAt(float mouseX) const
    {
        if(!isAudioLoaded)
            return HoverState::None;

        const float pos = rawPosValue->load();
        const float span = rawSpanValue->load();
        const float w = static_cast<float>(getWidth());

        const float posPx = pos * w;
        const float spanEndPx = posPx + (span * w);
        const bool wrap = spanEndPx > w;
        const float wrappedEnd = wrap ? (spanEndPx - w) : spanEndPx;

        const float tol = 6.0f;

        // Edge
        if(std::abs(mouseX - wrappedEnd) < tol)
            return HoverState::Edge;

        // Position
        if(std::abs(mouseX - posPx) < tol)
            return HoverState::Position;

        // Body
        if(!wrap)
        {
            if(mouseX > posPx + tol && mouseX < spanEndPx - tol)
                return HoverState::Body;
        }
        else
        {
            // deux segments
            if(mouseX > posPx + tol || mouseX < wrappedEnd - tol)
                return HoverState::Body;
        }

        return HoverState::None;
    }

}