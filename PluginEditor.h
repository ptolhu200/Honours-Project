/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "StyleSheet.h"

//==============================================================================
/**
*/
class HonoursPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    HonoursPluginAudioProcessorEditor(HonoursPluginAudioProcessor&,
        juce::AudioProcessorValueTreeState&);
    ~HonoursPluginAudioProcessorEditor();

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:

    HonoursPluginAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;


    // Initiate save and recomend buttons 
    juce::TextButton button1{ "Save D" }, button2{ "Save F" }, button3{ "Save R" }, button4{ "Rec D" }, button5{ "Rec F" }, button6{ "Rec R" };

    std::vector<juce::TextButton*> buttons =
    {
        &button1, &button2, &button3, &button4, &button5, &button6
    };

    juce::CustomLNF myCustomLNF;
    juce::CustomDial myDial;
    juce::CustomDial myDial1;
    juce::CustomDial myDial2;
    juce::CustomDial myDial3;
    //juce::CustomDial myDial4;
    juce::CustomDial myDial5;
    juce::CustomDial myDial6;
    juce::CustomDial myDial7;
    juce::CustomDial myDial8;
    juce::CustomDial myDial9;
    juce::CustomDial myDial10;
    juce::CustomDial myDial11;
    juce::CustomDial myDial12;
    juce::CustomDial myDial13;

    juce::Slider delayTimeDial;
    juce::Slider feedbackDial;
    juce::Slider wetDryDial;
    //juce::Slider sWidthDial;
    juce::Slider timeDial;
    juce::Slider lfoRateDial;
    juce::Slider depthDial;
    juce::Slider feedback2Dial;
    juce::Slider drywetDial;
    juce::Slider sizeDial;
    juce::Slider dampDial;
    juce::Slider widthDial;
    juce::Slider dwDial;
    juce::Slider gainDDial;
    juce::ComboBox modeSel;

    juce::Label delayTimeLabel;
    juce::Label feedbackLabel;
    juce::Label wetDryLabel;
    //juce::Label sWidthLabel;
    juce::Label timeLabel;
    juce::Label lfoRateLabel;
    juce::Label depthLabel;
    juce::Label feedback2Label;
    juce::Label drywetLabel;
    juce::Label sizeLabel;
    juce::Label dampLabel;
    juce::Label widthLabel;
    juce::Label dwLabel;
    juce::Label gainDLabel;
    juce::Label modeLabel;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> feedbackValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> wetDryValue;
    // std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sWidthValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> timeValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> lfoRateValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> depthValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> feedback2Value;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> drywetValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sizeValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> dampValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> widthValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> dwValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> gainDValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeChoice;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HonoursPluginAudioProcessorEditor)
};
