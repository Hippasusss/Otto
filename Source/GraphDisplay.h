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
#include "Graph.h"
class GraphDisplay    : public Component,
    public Timer
{
    public:
        static const int RING_BUFFER_SIZE = 50;

        GraphDisplay(Graph* newGraph);
        ~GraphDisplay();

        void paint (Graphics&) override;
        void timerCallback() override;
        void resized() override;
        CustomLookAndFeel2 lookAndFeel2;

    private:
        Graph* graph;
        AudioVisualiserComponent graphView;
        AudioBuffer<float> displayBuffer;
        AudioBuffer<float> dataBuffer;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDisplay)
};
