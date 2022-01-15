/*
  ==============================================================================

    Constants.cpp
    Created: 27 Feb 2020 9:17:19pm
    Author:  Danny Herbert

  ==============================================================================
*/


#pragma once
#include "JuceHeader.h"
namespace parameter_constants
{
	const String INPUT_GAIN_ID = "inputGain";
	const String OUTPUT_GAIN_ID = "outputGain";
	const String DRIVE_ID = "drive";
	const String FREQUENCY_ID = "frequency";
	const String RESONANCE_ID = "resonance";
	const String MIX_ID = "mix";
	const String ENV_SPEED_ID = "envSpeed";
	const String TWO_FOUR_POLE_ID = "twoFourPole";
	const String ENV_AMOUNT_ID = "envAmount";
}
namespace timer_constants 
{
	const int REFRESH_RATE = 60;
}

namespace colour_constants
{
	const Colour main = Colours::lightblue;
	const Colour lightMain = juce::Colour(0xffe0f4ff);
	const Colour backGround = Colours::white;
	const Colour red = Colours::salmon;
}
