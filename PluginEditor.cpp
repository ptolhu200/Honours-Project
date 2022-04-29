/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HonoursPluginAudioProcessorEditor::HonoursPluginAudioProcessorEditor
(HonoursPluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), audioProcessor(p), treeState(vts) /*apvts(vts)*/
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(550, 410);


    // save buttons

   /* for (auto i = 0; i < buttons.size(); i++)
    {
        addAndMakeVisible(buttons[i]);
        buttons[i]->setClickingTogglesState(true);

            if (i == 0)
            {
                buttons[i]->setBounds(550, 40, 40, 40);
            }


    }
    */

    // mode selection 
    modeSel.addItem("Delay", 1);
    modeSel.addItem("Flanger", 2);
    modeSel.addItem("Reverb", 3);
    modeChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "mode", modeSel);
    addAndMakeVisible(&modeSel);

    //Delay Dials 
    // delayTime
    delayTimeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "delayTime", myDial);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial.setRange(0.1f, 4000.0f, 1.0f);
    myDial.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial.setTextValueSuffix(" ms");
    addAndMakeVisible(myDial);

    // feedback 
    feedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "feedback", myDial1);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial1.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial1.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial1.setRange(0.0f, 0.80f, 0.01f);
    myDial1.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 55, 15);
    myDial1.setTextValueSuffix(" %");
    addAndMakeVisible(myDial1);

    // wet/dry 
    wetDryValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "wetDry", myDial2);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial2.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial2.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial2.setRange(0.0f, 1.0f, 0.01f);
    myDial2.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 55, 15);
    myDial2.setTextValueSuffix(" %");
    addAndMakeVisible(myDial2);

    // Chorus Dial

    // chorus time 
    timeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "time", myDial5);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial5.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial5.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial5.setRange(7.0f, 10.0f, 0.01f);
    myDial5.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial5.setTextValueSuffix(" ms");
    addAndMakeVisible(myDial5);

    // chorus lfo rate
    lfoRateValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "lfoRate", myDial6);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial6.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial6.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial6.setRange(0.0f, 20.0f, 0.01f);
    myDial6.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial6.setTextValueSuffix(" Hz");
    addAndMakeVisible(myDial6);

    // chorus depth
    depthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "depth", myDial10);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial10.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial10.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial10.setRange(0.0f, 1.0f, 0.01f);
    myDial10.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial10.setTextValueSuffix(" %");
    addAndMakeVisible(myDial10);

    // chorus feeback
    feedback2Value = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "feedback2", myDial12);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial12.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial12.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial12.setRange(-0.85f, 0.85f, 0.01f);
    myDial12.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial12.setTextValueSuffix(" %");
    addAndMakeVisible(myDial12);

    // chorus dry wet 
    drywetValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "drywet", myDial13);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial13.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial13.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial13.setRange(0.0f, 1.0f, 0.01f);
    myDial13.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial13.setTextValueSuffix(" %");
    addAndMakeVisible(myDial13);

    // Reverb Dials
    // rvb size
    sizeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "size", myDial7);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial7.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial7.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial7.setRange(0.0f, 1.0f, 0.1f);
    myDial7.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial7.setTextValueSuffix(" %");
    addAndMakeVisible(myDial7);

    // rvb dampness
    dampValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "damp", myDial8);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial8.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial8.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial8.setRange(0.0f, 1.0f, 0.1f);
    myDial8.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial8.setTextValueSuffix(" %");
    addAndMakeVisible(myDial8);

    // rvb width
    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "width", myDial9);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial9.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial9.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial9.setRange(0.0f, 1.0f, 0.1f);
    myDial9.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial9.setTextValueSuffix(" %");
    addAndMakeVisible(myDial9);

    // rvb dry / wet
    dwValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "dw", myDial11);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial11.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial11.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial11.setRange(0.0f, 1.0f, 0.1f);
    myDial11.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 75, 15);
    myDial11.setTextValueSuffix(" %");
    addAndMakeVisible(myDial11);


    // Gain in dB 
    gainDValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
        (treeState, "gainD", myDial3);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLNF);
    myDial3.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    myDial3.setVelocityModeParameters(2.0, 0.01, 0.0, true);
    myDial3.setRange(-12.0f, 12.0f, 0.01f);
    myDial3.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 55, 15);
    myDial3.setTextValueSuffix(" dB");
    addAndMakeVisible(myDial3);

    addAndMakeVisible(modeLabel);
    modeLabel.setText("Mode", juce::dontSendNotification);
    modeLabel.attachToComponent(&modeSel, true);

}


HonoursPluginAudioProcessorEditor::~HonoursPluginAudioProcessorEditor()
{
}

//==============================================================================
void HonoursPluginAudioProcessorEditor::paint(juce::Graphics& g)
{

    g.fillAll(juce::Colours::lightslategrey);
    g.setColour(juce::Colours::black);

    // mode and gain section 
    // renctangular pannel for mode selection and gain control 
    juce::Rectangle<int> area(350, 0, 230, 160);
    g.setColour(juce::Colours::silver);
    g.fillRect(area);


    // title text settings for Delay 
    g.setFont(20.0f);
    g.setColour(juce::Colours::black);
    g.drawFittedText("DELAY", 60, 20, 200, 15, juce::Justification::centred, 1, 0.0f);


    // Delay Time, Feedback, wet/dry control labels 
    g.setFont(15);
    g.drawFittedText("time", 30, 90, 20, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Fb", 140, 90, 10, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("D/W", 240, 90, 20, 10, juce::Justification::centred, 1, 0.0f);

    //Title text settings for Chorus 

    g.setFont(20.0f);
    g.drawFittedText("FLANGER", 60, 150, 200, 15, juce::Justification::centred, 1, 0.0f);

    // CHORUS Control labels 
    g.setFont(15);
    // g.drawFittedText("width", 35, 220, 40, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("time", 20, 220, 30, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("LFO", 130, 220, 40, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("depth", 235, 220, 40, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Fb", 345, 220, 40, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("D/W", 455, 220, 40, 10, juce::Justification::centred, 1, 0.0f);

    //Title text settings for Reverb 

    g.setFont(20.0f);
    g.drawFittedText("REVERB", 60, 280, 200, 15, juce::Justification::centred, 1, 0.0f);

    // REVERB Control labels 
    g.setFont(15);
    g.drawFittedText("size", 25, 350, 20, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("damp", 130, 350, 30, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("width", 235, 350, 30, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("D/W", 350, 350, 30, 10, juce::Justification::centred, 1, 0.0f);

    // Gain Control Labels 
    g.drawFittedText("dB", 420, 90, 20, 10, juce::Justification::centred, 1, 0.0f);

}

void HonoursPluginAudioProcessorEditor::resized()
{
    // lay out the positions of components/controls

    myDial.setBounds(10, 40, 100, 100);
    myDial1.setBounds(120, 40, 100, 100);
    myDial2.setBounds(230, 40, 100, 100);
    myDial3.setBounds(400, 40, 100, 100);
    //myDial4.setBounds(10, 170, 100, 100);
    myDial5.setBounds(10, 170, 100, 100);
    myDial6.setBounds(120, 170, 100, 100);
    myDial7.setBounds(10, 300, 100, 100);
    myDial8.setBounds(120, 300, 100, 100);
    myDial9.setBounds(230, 300, 100, 100);
    myDial10.setBounds(230, 170, 100, 100);
    myDial11.setBounds(340, 300, 100, 100);
    myDial12.setBounds(340, 170, 100, 100);
    myDial13.setBounds(450, 170, 100, 100);

    //set bounds for mode selection box 
    modeSel.setBounds(420, 20, 75, 25);


    // save buttons

    addAndMakeVisible(button1);
    button1.setClickingTogglesState(true);
    button1.setBounds(10, 20, 50, 25);

    addAndMakeVisible(button2);
    button2.setClickingTogglesState(true);
    button2.setBounds(10, 150, 50, 25);

    addAndMakeVisible(button3);
    button3.setClickingTogglesState(true);
    button3.setBounds(10, 280, 50, 25);

    // Recommendation Buttons

    addAndMakeVisible(button4);
    button4.setClickingTogglesState(true);
    button4.setBounds(270, 20, 50, 25);

    addAndMakeVisible(button5);
    button5.setClickingTogglesState(true);
    button5.setBounds(270, 150, 50, 25);

    addAndMakeVisible(button6);
    button6.setClickingTogglesState(true);
    button6.setBounds(400, 280, 50, 25);

}

