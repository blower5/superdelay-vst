#pragma once

#include "JuceHeader.h"

class FilterDelay 
{
public:
    
    FilterDelay();
    ~FilterDelay();
    
    void prepareToPlay(float inSampleRate, int samplesPerBlock);
    
    void processBlock(juce::AudioBuffer<float>& inBuffer);
    
    void calculateIIR(float inFilterFreq, float inFilterQ);

    double getResponse(float inFreq);
    juce::Array<float> getBandValues();
    /* pinged by scope visualizer */
    float getSampleFromNormalized(float inX);

    /* 
     // inDelayTimeSeconds [0, 4]
     // inFBPercent  [0, 1.2]
     // inDryWetPercent    [0, 1]
     */
    void setDelayParameters(float inDelayTimeSeconds, float inFBPercent, float inDryWetPercent, float inFBInverted, float inStereo);
    void setFilterParameters(float inFilterFreq, float inFilterQ);

private:

    /* Buffers */
    juce::AudioBuffer<float> mDelayBuffer;

    /* Playheads */
    int mDelayWritehead = 0;
    float mFBLeft = 0;
    float mFBRight = 0;

    float mSampleRate = 44100.f;
    const float mMaxDelayTimeSeconds = 4.f;
    
    float mLastFilterFreq = 0.f;
    float mLastFilterQ = 0.5f;

    juce::dsp::IIR::Filter<float> mIIRLeft;
    juce::dsp::IIR::Filter<float> mIIRRight;

    //in memoriam fir bandpass :((
    //juce::Array<float> mFIRArray = juce::Array<float>(.352, .352, .258, .133, .008, -.094, -.148, -.172);

    /* Smoothed Values */
    juce::LinearSmoothedValue<float> mSmoothedDryWet;
    juce::LinearSmoothedValue<float> mSmoothedFB;
    juce::LinearSmoothedValue<float> mSmoothedTime;

    //juce::LinearSmoothedValue<float> mSmoothedTimeIsLocked;
    juce::LinearSmoothedValue<float> mSmoothedFBInverted;
    juce::LinearSmoothedValue<float> mSmoothedStereo;

    float normalizeFreq(float inFreq);
    float freqFromNormalized(float inNormalizedFreq);
};