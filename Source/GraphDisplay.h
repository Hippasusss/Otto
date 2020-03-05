/*
  ==============================================================================

    GraphDisplay.h
    Created: 3 Mar 2020 11:27:17pm
    Author:  Danny Herbert

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StackFIFO.h"

//==============================================================================
/*
*/
class GraphDisplay    : public Component,
						public Timer
{
public:
    GraphDisplay() = delete;
    explicit GraphDisplay(const StackFIFO<float, 20>& fifoToDraw);
    ~GraphDisplay();

    void paint (Graphics&) override;
    void resized() override;
    void updateListPointer(std::list<float>* list);

     void timerCallback() override;
private:
    const StackFIFO<float, 20>& valuesToDraw;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDisplay)
};
