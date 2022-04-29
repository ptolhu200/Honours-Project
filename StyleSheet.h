/*
  ==============================================================================

    StyleSheet.h
    Created: 6 Apr 2022 12:31:59pm
    Author:  Peter Tolhurst

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace juce
{
    class CustomLNF : public juce::LookAndFeel_V4
    {
    public:
        // override code for customzing 
        void drawRotarySlider(Graphics&, int x, int y, int width, int height, float sliderPosProportional,
            float rotaryStartAngle, float rotaryEndAngle, Slider&);
    };

    class CustomDial : public Slider
    {
    public:
        CustomDial()
        {
            auto rotaryParams = getRotaryParameters();
            rotaryParams.startAngleRadians = juce::MathConstants<float>::pi * 1.5f;
            rotaryParams.endAngleRadians = juce::MathConstants<float>::pi * 3.0f;
            setRotaryParameters(rotaryParams);

            // set the style of slider (aiming for rotary style with vintage synth knob meets ableton look) 

            setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
            setRange(0.0f, 100.0f);
            setValue(30.0f);
            setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
            setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::orange);
            setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange);
        }


    private: 
    };


}

