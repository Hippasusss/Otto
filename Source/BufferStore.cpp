/*
  ==============================================================================

    BufferStore.cpp
    Created: 8 Mar 2020 4:40:40pm
    Author:  Danny Herbert

  ==============================================================================
*/

#include "BufferStore.h"


BufferStore::BufferStore() : block(buffer)
{
}

void BufferStore::prepare(const dsp::ProcessSpec& spec)
{
    buffer.setSize(spec.numChannels, spec.maximumBlockSize);
    buffer.clear();
    block = dsp::AudioBlock<float>(buffer);
}

void BufferStore::process(const dsp::ProcessContextReplacing<float>& context)
{
    block.copyFrom(context.getInputBlock());
}

void BufferStore::reset()
{
    block.clear();
}

dsp::AudioBlock<float>* BufferStore::getAudioBlockPointer()
{
    return &block;
}
