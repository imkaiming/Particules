#include "ChannelMixer.h"

//AudioBuffer ChannelMixer::downmix(const AudioBuffer& input, const int outputNumChannels) { return AudioBuffer(); }
//void ChannelMixer::computeMixMatrix(int inputChannels, int outputChannels, float* matrix, int maxChannels) noexcept {}
//void ChannelMixer::applyMix(
//    const AudioBuffer& input, AudioBuffer& output, const float* matrix, int inputChannels, int outputChannels) noexcept
//{
//}

namespace particules
{
    // DOWNMIX Standard ITU-R BS.775
    AudioBuffer ChannelMixer::downmix(const AudioBuffer& inputBuffer) const
    {
        const int inputChannels = inputBuffer.getNumChannels();
        const int numSamples = inputBuffer.getNumSamples();

        jassert(inputChannels != 0 && targetChannels != 0);

        if(inputChannels == targetChannels)
            return inputBuffer;

        juce::AudioBuffer<float> result(targetChannels, numSamples);

        // Mono to Stereo
        if(inputChannels == 1 && targetChannels == 2)
        {
            result.copyFrom(0, 0, inputBuffer, 0, 0, numSamples);
            result.copyFrom(1, 0, inputBuffer, 0, 0, numSamples);
            return result;
        }

        // Stereo to Mono
        if(inputChannels == 2 && targetChannels == 1)
        {
            const float* l = inputBuffer.getReadPointer(0);
            const float* r = inputBuffer.getReadPointer(1);
            float* dest = result.getWritePointer(0);
            juce::FloatVectorOperations::clear(dest, numSamples);
            juce::FloatVectorOperations::addWithMultiply(dest, l, 0.5f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(dest, r, 0.5f, numSamples);
            return result;
        }

        // 3.0 to Stereo
        if(inputChannels == 3 && targetChannels == 2)
        {
            const float* l = inputBuffer.getReadPointer(0);
            const float* r = inputBuffer.getReadPointer(1);
            const float* c = inputBuffer.getReadPointer(2);

            float* destL = result.getWritePointer(0);
            float* destR = result.getWritePointer(1);

            juce::FloatVectorOperations::copy(destL, l, numSamples);
            juce::FloatVectorOperations::copy(destR, r, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, c, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, c, 0.707f, numSamples);

            return result;
        }

        // Quad to Stereo
        if(inputChannels == 4 && targetChannels == 2)
        {
            const float* l = inputBuffer.getReadPointer(0);
            const float* r = inputBuffer.getReadPointer(1);
            const float* ls = inputBuffer.getReadPointer(2);
            const float* rs = inputBuffer.getReadPointer(3);

            float* destL = result.getWritePointer(0);
            float* destR = result.getWritePointer(1);

            juce::FloatVectorOperations::copy(destL, l, numSamples);
            juce::FloatVectorOperations::copy(destR, r, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, ls, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, rs, 0.707f, numSamples);

            return result;
        }

        // 5.0 to Stereo
        if(inputChannels == 5 && targetChannels == 2)
        {
            const float* l = inputBuffer.getReadPointer(0);
            const float* r = inputBuffer.getReadPointer(1);
            const float* c = inputBuffer.getReadPointer(2);
            const float* ls = inputBuffer.getReadPointer(3);
            const float* rs = inputBuffer.getReadPointer(4);

            float* destL = result.getWritePointer(0);
            float* destR = result.getWritePointer(1);

            juce::FloatVectorOperations::copy(destL, l, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, c, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, ls, 0.707f, numSamples);

            juce::FloatVectorOperations::copy(destR, r, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, c, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, rs, 0.707f, numSamples);

            return result;
        }

        // 5.1 to Stereo
        if(inputChannels == 6 && targetChannels == 2)
        {
            const float* l = inputBuffer.getReadPointer(0);
            const float* r = inputBuffer.getReadPointer(1);
            const float* c = inputBuffer.getReadPointer(2);
            const float* lfe = inputBuffer.getReadPointer(3);
            const float* ls = inputBuffer.getReadPointer(4);
            const float* rs = inputBuffer.getReadPointer(5);

            float* destL = result.getWritePointer(0);
            float* destR = result.getWritePointer(1);

            juce::FloatVectorOperations::copy(destL, l, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, c, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, ls, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, lfe, 0.3f, numSamples);

            juce::FloatVectorOperations::copy(destR, r, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, c, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, rs, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, lfe, 0.3f, numSamples);

            return result;
        }

        // 7.1 to Stereo (Dolby Pro Logic II)
        if(inputChannels == 8 && targetChannels == 2)
        {
            const float* l = inputBuffer.getReadPointer(0);
            const float* r = inputBuffer.getReadPointer(1);
            const float* c = inputBuffer.getReadPointer(2);
            const float* lfe = inputBuffer.getReadPointer(3);
            const float* ls = inputBuffer.getReadPointer(4);
            const float* rs = inputBuffer.getReadPointer(5);
            const float* lb = inputBuffer.getReadPointer(6);
            const float* rb = inputBuffer.getReadPointer(7);

            float* destL = result.getWritePointer(0);
            float* destR = result.getWritePointer(1);

            juce::FloatVectorOperations::copy(destL, l, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, c, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, ls, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, lb, 0.5f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destL, lfe, 0.3f, numSamples);

            juce::FloatVectorOperations::copy(destR, r, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, c, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, rs, 0.707f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, rb, 0.5f, numSamples);
            juce::FloatVectorOperations::addWithMultiply(destR, lfe, 0.3f, numSamples);

            return result;
        }

        // Fallback
        float gain = 1.0f / std::sqrt(static_cast<float>(inputChannels));
        for(int out = 0; out < targetChannels; ++out)
        {
            float* dest = result.getWritePointer(out);

            for(int in = 0; in < inputChannels; ++in)
            {
                const float* src = inputBuffer.getReadPointer(in);
                juce::FloatVectorOperations::addWithMultiply(dest, src, gain, numSamples);
            }
        }

        return result;
    }
    void ChannelMixer::setTargetChannel(int channels) noexcept
    {
        jassert(channels > 0 && channels < MAX_CHANNELS);
        targetChannels = channels;
    }
}
/*
AudioBuffer ChannelMixer::downmix(const AudioBuffer& inputBuffer, const int outputNumChannels)
{
    DBG("down mix buffer");
    const int inputNumChannels = inputBuffer.getNumChannels();
    const int inputNumSamples = inputBuffer.getNumSamples();

    if(inputNumChannels <= 0 || inputNumChannels > MAX_CHANNELS || outputNumChannels <= 0 || outputNumChannels > MAX_CHANNELS)
        return {};

    AudioBuffer outputBuffer(outputNumChannels, inputNumSamples);
    outputBuffer.clear();

    // row-major flattened matrix : simulate matrix[out][in]
    // easier to pass with ptr
    float matrix[MAX_CHANNELS * MAX_CHANNELS] = {0.0f};

    computeMixMatrix(inputNumChannels, outputNumChannels, matrix, MAX_CHANNELS);

    applyMix(inputBuffer, outputBuffer, matrix, inputNumChannels, outputNumChannels);

    return outputBuffer;
}

void ChannelMixer::computeMixMatrix(int inputNumChannels, int outputNumChannels, float* matrix, int maxChannels) noexcept
{
    for(int i = 0; i < maxChannels * maxChannels; ++i)
        matrix[i] = 0.0f;

    auto W = [matrix, maxChannels](int out, int in) -> float& { return matrix[out * maxChannels + in]; };

    if(inputNumChannels == outputNumChannels)
    {
        for(int i = 0; i < inputNumChannels; ++i)
            W(i, i) = 1.0f;
        return;
    }

    // Mono to Stereo
    if(inputNumChannels == 1 && outputNumChannels == 2)
    {
        W(0, 0) = 1.0f;
        W(1, 0) = 1.0f;
        return;
    }

    // Stereo to Mono
    if(inputNumChannels == 2 && outputNumChannels == 1)
    {
        W(0, 0) = 0.5f;
        W(0, 1) = 0.5f;
        return;
    }

    // Stereo to Stereo
    if(inputNumChannels >= 2 && outputNumChannels == 2)
    {
        W(0, 0) = 1.0f;
        W(1, 1) = 1.0f;
        return;
    }

    // 4.0 to Stereo
    if(inputNumChannels == 4 && outputNumChannels == 2)
    {
        // L, R, Ls, Rs
        W(0, 0) = 0.5f;
        W(0, 2) = 0.5f;

        W(1, 1) = 0.5f;
        W(1, 3) = 0.5f;
        return;
    }

    // 5.1 to Stereo
    if(inputNumChannels == 6 && outputNumChannels == 2)
    {
        // L, R, C, LFE, Ls, Rs

        // Left
        W(0, 0) = 1.0f; // L
        W(0, 2) = 0.707f; // C
        W(0, 4) = 0.5f; // Ls

        // Right
        W(1, 1) = 1.0f; // R
        W(1, 2) = 0.707f; // C
        W(1, 5) = 0.5f; // Rs

        // LFE ignored
        return;
    }

    // Fallback
    for(int out = 0; out < outputNumChannels; ++out)
    {
        const int in = out % inputNumChannels;
        W(out, in) = 1.0f;
    }
}

void ChannelMixer::applyMix(
    const AudioBuffer& input, AudioBuffer& output, const float* matrix, int inputNumChannels, int outputNumChannels) noexcept
{
    const int numSamples = input.getNumSamples();

    const float* inputPtrs[MAX_CHANNELS];
    float* outputPtrs[MAX_CHANNELS];

    for(int channel = 0; channel < inputNumChannels; ++channel)
        inputPtrs[channel] = input.getReadPointer(channel);

    for(int channel = 0; channel < outputNumChannels; ++channel)
        outputPtrs[channel] = output.getWritePointer(channel);

    for(int i = 0; i< outputNumChannels; ++i)
    {
        float* dest = outputPtrs[i];
        const float* row = &matrix[i * MAX_CHANNELS];

        juce::FloatVectorOperations::clear(dest, numSamples);

        for(int in = 0; in < inputNumChannels; ++in)
        {
            float coeff = row[in];
            if(coeff == 0.0f)
                continue;

            const float* src = inputPtrs[in];
            juce::FloatVectorOperations::addWithMultiply(dest, src, coeff, numSamples);
        }
    }
}

*/