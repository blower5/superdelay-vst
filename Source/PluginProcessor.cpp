#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuperdelayAudioProcessor::SuperdelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    _constructValueTreeState();

    mDryWet = mValueTreeState->getRawParameterValue("dry_wet");
    mFeedback = mValueTreeState->getRawParameterValue("fb");
    mTime = mValueTreeState->getRawParameterValue("time");
    mTimeSNES = mValueTreeState->getRawParameterValue("timesnes");

    mTimeIsLocked = mValueTreeState->getRawParameterValue("timeislocked");
    mFBInverted = mValueTreeState->getRawParameterValue("fbinv");
    mStereo = mValueTreeState->getRawParameterValue("stereo");

    mFilterFreq = mValueTreeState->getRawParameterValue("filterfreq");
    mFilterBand = mValueTreeState->getRawParameterValue("filterband");
}

SuperdelayAudioProcessor::~SuperdelayAudioProcessor()
{
}

//==============================================================================
void SuperdelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    mFilterDelay.prepareToPlay(sampleRate, samplesPerBlock);
}

void SuperdelayAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{   

    float delaytime;
    (*mTimeIsLocked == 1.f) 
    ?
        delaytime = *mTimeSNES * .016 //index to ms
    :
        delaytime = *mTime;

    mFilterDelay.setDelayParameters(delaytime, *mFeedback, *mDryWet, *mFBInverted, *mStereo);
    mFilterDelay.setFilterParameters(*mFilterFreq, *mFilterBand);
    
    // --------------------------
    // Sine Tone Generation Start
    // --------------------------
    /*float* left_channel = buffer.getWritePointer(0);
    float* right_channel = buffer.getWritePointer(1);
    
    float sine_hz = 440.f;
    float phase_delta = sine_hz / getSampleRate();
    
    for (int sample_idx = 0; sample_idx < buffer.getNumSamples(); sample_idx++) {
        
        float sine_out = std::sin(mSineTonePhase * 2 * juce::MathConstants<float>::pi);
                
        mSineTonePhase += phase_delta;
        
        if (mSineTonePhase > 1.f) {
            mSineTonePhase -= 1.f;
        }
        
        left_channel[sample_idx] = sine_out;
        right_channel[sample_idx] = sine_out;
    }*/
    // ------------------------
    // Sine Tone Generation End
    // ------------------------
    
    // --------------------
    // Process Our Delay
    // --------------------
    mFilterDelay.processBlock(buffer);
}

//==============================================================================
const juce::String SuperdelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SuperdelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SuperdelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SuperdelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SuperdelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SuperdelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SuperdelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SuperdelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SuperdelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void SuperdelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void SuperdelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SuperdelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


//==============================================================================
bool SuperdelayAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SuperdelayAudioProcessor::createEditor()
{
    return new SuperdelayAudioProcessorEditor (*this);
}

//==============================================================================
void SuperdelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = mValueTreeState->copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SuperdelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    mValueTreeState->replaceState(juce::ValueTree::fromXml(*xmlState));
}

void SuperdelayAudioProcessor::_constructValueTreeState()
{
    mValueTreeState.reset(new juce::AudioProcessorValueTreeState(*this, nullptr, juce::Identifier("My_First_Plugin"),
        {
            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("dry_wet", 1), // parameterID
                                                         "Dry/Wet", // parameter name
                                                         0.0f,   // minimum value
                                                         1.0f,   // maximum value
                                                         0.5f),   // default value

            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("fb", 1), // parameterID
                                                         "Feedback", // parameter name
                                                         0.0f,   // minimum value
                                                         1.2f,   // maximum value
                                                         0.5f),   // default value
            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("time", 1), // parameterID
                                                         "Time (ms)", // parameter name
                                                         juce::NormalisableRange<float>(
                                                             0.0005f, //minimum value
                                                             4.0f, //maximum value
                                                             0.f, //interval
                                                             .25f), //skew
                                                         0.096f, //default
                                                         juce::String(),
                                                         juce::AudioParameterFloat::Category::genericParameter,
                                                         [](float value, int)
                                                         {
                                                             if (value < .001f) return juce::String(value * 1000, 3) + "ms";
                                                             if (value < 1.f) return juce::String(value * 1000, 1) + "ms";
                                                             return juce::String(value,3) + "s";
                                                         }),
            std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("timesnes", 1), // parameterID
                                                         "Time (SNES)", // parameter name
                                                         juce::StringArray
                                                         {
                                                            "16",
                                                            "32",
                                                            "48",
                                                            "64",
                                                            "80",
                                                            "96",
                                                            "112",
                                                            "128",
                                                            "144",
                                                            "160",
                                                            "176",
                                                            "192",
                                                            "208",
                                                            "224",
                                                            "240",
                                                            "240"
                                                         },
                                                         5, //default index
                                                         juce::String(),
                                                         [](int index, int length) {return juce::String(index * 16) + "ms"; },
                                                         [](juce::String index) {return index.getIntValue(); }),
        std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("timebpm", 1), // parameterID
                                                         "Time (BPM)", // parameter name
                                                         juce::StringArray
                                                         {
                                                            "1/32",
                                                            "1/32t",
                                                            "1/32d",
                                                            "1/16",
                                                            "1/16",
                                                            "1/16",
                                                            "1/8",
                                                            "1/8t",
                                                            "1/8d",
                                                            "1/4",
                                                            "1/4t",
                                                            "1/4d",
                                                            "1/2",
                                                            "1/2t",
                                                            "1/2d",
                                                            "1",
                                                            "1t",
                                                            "1d",
                                                            "2",
                                                            "2t",
                                                            "2d"
                                                         },
                                                         7), //default index
            std::make_unique<juce::AudioParameterBool>(juce::ParameterID("timeislocked", 1), // parameterID
                                                         "Delay Time Lock", // parameter name
                                                         false),   // default value
            std::make_unique<juce::AudioParameterBool>(juce::ParameterID("fbinv", 1), // parameterID
                                                         "Invert Feedback", // parameter name
                                                         false),   // default value
            std::make_unique<juce::AudioParameterBool>(juce::ParameterID("stereo", 1), // parameterID
                                                         "Surround", // parameter name
                                                         false),   // default value

            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("filterfreq", 1), // parameterID
                                                         "Filter Frequency", // parameter name
                                                         juce::NormalisableRange<float>(
                                                             0.f,
                                                             1.f,
                                                             0.f),
                                                         .7f,
                                                         juce::String(),
                                                         juce::AudioParameterFloat::Category::genericParameter,
                                                         [](float val, int) {return juce::String(10*powf(2200.,val)) + " Hz"; }),
            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("filterband", 1), // parameterID
                                                         "Filter Q", // parameter name
                                                         juce::NormalisableRange<float>(
                                                            0.f,
                                                            1.f,
                                                            0.f),
                                                         0.5f, 
                                                         juce::String(),
                                                         juce::AudioParameterFloat::Category::genericParameter,
                                                         [](float val, int) {return juce::String((powf(60, val) - .95f) / 50.f); })
        }));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SuperdelayAudioProcessor();
}
