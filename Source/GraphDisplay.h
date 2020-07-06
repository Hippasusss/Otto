/*
   ==============================================================================

   GraphDisplay.h
Created: 3 Mar 2020 11:27:17pm
Author:  Danny Herbert

==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "CustomLookAndFeel.h"
#include "RingBuffer.h"
class GraphDisplay    : public Component,
    public Timer
{
    public:
        static const int RING_BUFFER_SIZE = 50;

        GraphDisplay();
        ~GraphDisplay();

        void paint (Graphics&) override;
        void timerCallback() override;
        void resized() override;
        AudioVisualiserComponent graph;
        CustomLookAndFeel2 lookAndFeel2;

    private:
        RingBufferAudio<float> valuesToDraw;
        float valueOffset;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDisplay)
};
