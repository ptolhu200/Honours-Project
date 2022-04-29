/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================

class HonoursPluginAudioProcessor : public juce::AudioProcessor,
    /// processor class will now inherit from AudioProcessingValueTreeState::Listener
    public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    HonoursPluginAudioProcessor();
    ~HonoursPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // declares function to include with paramters 
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    // new save feature
    bool OutputStream();


    juce::AudioProcessorValueTreeState treeState;

private:

    // save buttons 


    float pastGain;

    // buffer increased to 144000 to accomodate projects at 48000. 
    juce::dsp::DelayLine<float> mDelayLine{ 192000 }; // changed to 4 x 48000 to accomodate projects with sample rate of 48000 as maximum delay is 4s

    float mDelayTime = 1000.0f;
    float mFeedback = 0.3f;
    float mWetDry = 0.3f;
    float mGainD = 0.3f;
    float mode = 0.0f;
    float mSampleRate = 0.0f;


    float roomSize = 50.0f;
    float damping = 50.0f;
    float width = 50.0f;
    float wetLevel = 50.0f;
    float dryLevel = 50.0f;


    // Chorus 
    juce::dsp::Chorus<float> mFlanger;


    // Reverb 
    juce::dsp::Reverb::Parameters params;
    juce::dsp::Reverb mReverb;

    // save buttons 
    float button1;
    float button2;
    float button3;

    // recommendation buttons
    float button4;
    float button5;
    float button6;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HonoursPluginAudioProcessor)
};
