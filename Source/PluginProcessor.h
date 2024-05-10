#pragma once

#include <JuceHeader.h>
#include "FilterDelay.h"

//==============================================================================
/**
*/
class SuperdelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SuperdelayAudioProcessor();
    ~SuperdelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //Expose value tree
    juce::AudioProcessorValueTreeState& getVTS()
    {
        return *mValueTreeState;
    };

    /* pinged by visualizer */
    double getFilterResponse(float inFreq)
    {
        return mFilterDelay.getResponse(inFreq);
    }

    //unused
    juce::Array<float> getBandValues()
    {
        return mFilterDelay.getBandValues();
    }

    /* pinged by scope visualizer */
    float getSampleFromNormalized(float inX)
    {
        return mFilterDelay.getSampleFromNormalized(inX);
    }


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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:

    //initialize efx here when ready
    FilterDelay mFilterDelay;

    float mSineTonePhase = 0.f;

    std::atomic<float>* mDryWet = nullptr;
    std::atomic<float>* mFeedback = nullptr;
    std::atomic<float>* mTime = nullptr;
    std::atomic<float>* mTimeSNES = nullptr;

    std::atomic<float>* mTimeIsLocked = nullptr;
    std::atomic<float>* mFBInverted = nullptr;
    std::atomic<float>* mStereo = nullptr;

    std::atomic<float>* mFilterFreq = nullptr;
    std::atomic<float>* mFilterBand = nullptr;

    std::unique_ptr<juce::AudioProcessorValueTreeState> mValueTreeState;

    // value tree construction function, this needs to be called in the constructor of the plugin.
    void _constructValueTreeState();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperdelayAudioProcessor)
};
