/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HonoursPluginAudioProcessor::HonoursPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ), treeState(*this, nullptr, juce::Identifier("params"), // declare parameter and starting points
        // Delay Params
        // Delay Time
        { std::make_unique<juce::AudioParameterFloat>("delayTime", "Delay", 0.1f, 4000.0f, 1000.f),
        // Feedback
        std::make_unique<juce::AudioParameterFloat>("feedback", "Feedback", 0.0f, 0.80f, 0.3f),
        // WET /DRY MIX
        std::make_unique<juce::AudioParameterFloat>("wetDry", "Wet/Dry", 0.0f, 1.0f, 0.5f),

        // Chorus Params
        // c1
        std::make_unique<juce::AudioParameterFloat>("time", "time", 7.0f, 10.0f, 7.0f),
        //c2
        std::make_unique<juce::AudioParameterFloat>("lfoRate", "lfo rate", 0.01f, 25.0f, 0.01f),
        // c3
        std::make_unique<juce::AudioParameterFloat>("depth", "depth", 0.0f, 1.0f, 0.0f),
        // c4
        std::make_unique<juce::AudioParameterFloat>("feedback2", "feedback 2", -0.85f, 0.85f, 0.0f),
        // c5
        std::make_unique<juce::AudioParameterFloat>("drywet", "dry wet", 0.0f, 1.0f, 0.5f),
        // Reverb Params 
        // rvb size
        std::make_unique<juce::AudioParameterFloat>("size", "size", 0.0f, 1.0f, 0.5f),
        //rvb dampness
        std::make_unique<juce::AudioParameterFloat>("damp", "damp", 0.0f, 1.0f, 0.5f),
        // rvb width
        std::make_unique<juce::AudioParameterFloat>("width", "width", 0.0f, 1.0f, 0.5f),
        // rvb dw
        std::make_unique<juce::AudioParameterFloat>("dw", "dw", 0.0f, 1.0f, 0.5),

        // gain in dB
        std::make_unique<juce::AudioParameterFloat>("gainD", "gain dB", -12.0f, 12.0f, 1.0f),
        // mode section defines the three effect types 
        std::make_unique<juce::AudioParameterChoice>("mode", "Effect Type",
        juce::StringArray("Delay", "Flanger", "Reverb"), 0) })


#endif
{
    const juce::StringArray params = { "delayTime", "feedback", "wetDry","time", "lfoRate", "depth", "feedback2", "drywet", "size", "damp", "width", "dw", "gainD", "mode" };

    for (int i = 0; i <= 13; ++i)
    {
        // adds a listener to each parameter in the array
        treeState.addParameterListener(params[i], this);
    }
}


HonoursPluginAudioProcessor::~HonoursPluginAudioProcessor()
{
}

//==============================================================================
const juce::String HonoursPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HonoursPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool HonoursPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool HonoursPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double HonoursPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HonoursPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HonoursPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HonoursPluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String HonoursPluginAudioProcessor::getProgramName(int index)
{
    return {};
}


void HonoursPluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void HonoursPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // set up prepare to play

    juce::dsp::ProcessSpec spec;

    // for use in delay processing ensuring delay is in miliseconds rather than samples 

    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    // creating sample rate based on DAW
    spec.sampleRate = sampleRate;
    mSampleRate = sampleRate;

    mDelayLine.reset();
    mDelayLine.prepare(spec);

    // flanger / chorus 
    mFlanger.reset();
    mFlanger.prepare(spec);

    // Calculate Previous Gain in dB
    pastGain = pow(10, *treeState.getRawParameterValue("gainD") / 20);

    // Reverb 
    int Mode = *treeState.getRawParameterValue("mode");

    // if ((Mode == 2))

       //  spec.numChannels = 1;

    mReverb.prepare(spec);
    //rightReverb.prepare(spec);

}

void HonoursPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HonoursPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void HonoursPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Get current gain to perform processing with 

    float currentGain = pow(10, *treeState.getRawParameterValue("gainD") / 20);

    // Recalling mode state from treestate
    int Mode = *treeState.getRawParameterValue("mode");

    // Wet / Dry varaible recall from treestate
    float mWetDry = *treeState.getRawParameterValue("wetDry");

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);

    // Gain processing 
    if (currentGain == pastGain)
    {
        buffer.applyGain(currentGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), pastGain, currentGain);
        pastGain = currentGain;
    }

    if ((Mode == 0))
    {
        for (int i = 0; i < buffer.getNumSamples(); i++)
        {
            // build basic delay with feedback
            // 
            // left channel = 0
            float inL = channelDataL[i];
            float temp1 = mDelayLine.popSample(0, (mDelayTime));

            // Right channel = 1
            float inR = channelDataR[i];
            float temp2 = mDelayLine.popSample(1, mDelayTime);

            // loop signals to L&R channels using pushSample
            mDelayLine.pushSample(0, inL + ((temp1 + temp2) * mFeedback));
            mDelayLine.pushSample(1, inR + ((temp2 + temp1) * mFeedback));

            // wet/dry mix
            // 1 - wetdry subtracts dry signal when fader increased to max
            // * wetdry duplicates wet signal when fader is inceased to max

            channelDataL[i] = ((inL * (1 - mWetDry)) + (temp1 * mWetDry)) * 0.33f;
            channelDataR[i] = ((inR * (1 - mWetDry)) + (temp2 * mWetDry)) * 0.33f;
        }
    }
    else if ((Mode == 1))  // Flanger
    {
        // to get classic chorus sounds delay time of between 7-8ms
        // Flanger Implementation
        juce::dsp::AudioBlock<float> block(buffer);
        auto processingContext = juce::dsp::ProcessContextReplacing<float>(block);
        mFlanger.process(processingContext);
    }
    else if ((Mode == 2)) // Reverb 
    {
        // sets up paramter treestate's
        params.roomSize = *treeState.getRawParameterValue("size");
        params.damping = *treeState.getRawParameterValue("damp");
        params.width = *treeState.getRawParameterValue("width");
        params.wetLevel = *treeState.getRawParameterValue("dw");
        params.dryLevel = 1.0f - *treeState.getRawParameterValue("dw");

        mReverb.setParameters(params);

        // creates reveerb buffer 
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        mReverb.process(context);

    }

}


//==============================================================================
bool HonoursPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HonoursPluginAudioProcessor::createEditor()
{
    return new HonoursPluginAudioProcessorEditor(*this, treeState);
}

//==============================================================================
void HonoursPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // save state for next session

    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void HonoursPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // recall state information from previous session 

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HonoursPluginAudioProcessor();
}


// Function called when paramter is changed 
void HonoursPluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "delayTime")
    {
        // Conversion from ms to Samples
        mDelayTime = (newValue / 1000.0f) * mSampleRate;
        mDelayLine.setDelay(mDelayTime);
    }

    else if (parameterID == "feedback")
    {
        mFeedback = newValue;
    }
    else if (parameterID == "time")
    {
        mFlanger.setCentreDelay(newValue); // Changes Delay Time
    }
    else if (parameterID == "lfoRate")
    {
        mFlanger.setRate(newValue); // Changes Rate of LFO
    }
    else if (parameterID == "depth")
    {
        mFlanger.setDepth(newValue); // Changes Depth of LFO to apply to Delay
    }
    else if (parameterID == "feedback2")
    {
        mFlanger.setFeedback(newValue); // Changes Feedback of the Delay
    }
    else if (parameterID == "drywet")
    {
        mFlanger.setMix(newValue); // Changes Balance between Dry and Wet Signals
    }
}

/*
        // Save Button Feature
        // if button is pressed > send parameter data to each effects datasheet

        //Delay

        if (button1 = true)
        {
            float newdelayTime = *treeState.getRawParameterValue("delayTime");
            float newfeedback = *treeState.getRawParameterValue("feedback");
            float newwetDry = *treeState.getRawParameterValue("wetDry");
        }
        
        flanger / chorus 
        if (button2 = true)
        {
            float newtime = *treeState.getRawParameterValue("time");
            float newlfoRate = *treeState.getRawParameterValue("lfoRate");
            float newdepth = *treeState.getRawParameterValue("depth");
            float newfeedback2 = *treeState.getRawParameterValue("feedback2");
            float newdrywet = *treeState.getRawParameterValue("drywet");
        }

        reverb 
        if (button3 = true)
        {
            float newsize = *treeState.getRawParameterValue("size");
            float newdamp = *treeState.getRawParameterValue("damp");
            float newwidth = *treeState.getRawParameterValue("width");
            float newdw = *treeState.getRawParameterValue("dw");
        }

        // aimed to implement OutputStream class here to store new parameter vales when save buttons where triggered
        // have been unable to figure out how to do so in the time since finding out about the OutputStream class

        // now calculate average for each parameter
        
        // send average value to parameter controls when recomendation buttons are hit

        delay
        if (button4 = true)
        {
            change delayTime to new average value 
            change feedback to new average value 
            change wetDry to new average value 
        }
     
        flanger 
        if (button5 = true)
        { 
            change time to new average value
            change lfoRate to new average value 
            change depth to new average value 
            change feedback2 to new average value 
            change drywet to new average value 
        }
        
        Reverb
         if (button6 = true)
         {
         change size to new average value 
         change damp to new average value 
         change width to new average value 
         change dw to new average value 
         }


        */