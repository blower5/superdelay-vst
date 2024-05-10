#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class FilterVisualizer : public juce::Component,
    public juce::Timer {
public:
    /* */
    FilterVisualizer(SuperdelayAudioProcessor* inProcessor);

    /* */
    void paint(juce::Graphics& g) override;

    /* */
    void timerCallback() override;

private:

    SuperdelayAudioProcessor* mProcessor;
};
/*
class ScopeVisualizer : public juce::Component,
    public juce::Timer {
public:

    ScopeVisualizer(SuperdelayAudioProcessor* inProcessor);
    void timerCallback() override;
    void handleAudio(juce::AudioBuffer<float>& buffer);

    void paint(juce::Graphics& g) override;


    

private:
    const int mScopeLength = 512;
    juce::Array<float> mFIFOSampleArray;
    SuperdelayAudioProcessor* mProcessor;
};
*/