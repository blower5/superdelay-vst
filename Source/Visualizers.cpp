#include "Visualizers.h"

FilterVisualizer::FilterVisualizer(SuperdelayAudioProcessor* inProcessor)
{
    mProcessor = inProcessor;
    startTimerHz(30);
}

void FilterVisualizer::timerCallback()
{
    repaint();
}

void FilterVisualizer::paint(juce::Graphics& g)
{


    g.setColour(juce::Colour(0xFF575659));
    g.fillRoundedRectangle(this->getLocalBounds().toFloat(), 4.f);
    //g.fillAll(juce::Colours::white);
    
    //draw frequency lines logarithmically
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 10; j++) { //10, 20... 90, 100, 200... 10000, 20000

            (j == 1) ?
                g.setColour(juce::Colour(0x909E9DA0)) :
                g.setColour(juce::Colour(0x489E9DA0));

            float freq = j * powf(10, i);
            freq = logf(juce::jlimit(0.001f, 21950.f, freq) / 10.f) / logf(2200.f);
            freq = juce::jmap(freq, 0.f, (float)getWidth());
            g.fillRect(freq, 0.f, 1.f, (float)getHeight());
        }
    }


    //code that draws response curve
    juce::Path p;

    for (int x = 0; x < getWidth(); x++) {
        float y_val = juce::jmap((float)x, 0.f, (float)getWidth(), 0.f, 1.f);
        y_val = mProcessor->getFilterResponse(y_val);
        y_val = 2. * y_val - 1.;

        if (x == 0) {
            p.startNewSubPath(x, juce::jmap(y_val, -1.f, 1.f, (float)getHeight(), 0.f));
        }
        else {
            p.lineTo(x, juce::jmap(y_val, -1.f, 1.f, (float)getHeight(), 0.f));
        }
    }
   
    g.setColour(juce::Colour(0xFFAAA7DE));
    g.strokePath(p, juce::PathStrokeType(1.5));
}

//===============================================================================================
/*
 
ScopeVisualizer::ScopeVisualizer(SuperdelayAudioProcessor* inProcessor)
{
    mProcessor = inProcessor;
    startTimerHz(30);
}

void ScopeVisualizer::timerCallback()
{
    repaint();
}

void ScopeVisualizer::handleAudio(juce::AudioBuffer<float>& buffer)
{
    mFIFOSampleArray.clearQuick();
    int buffersize = buffer.getNumSamples();
    for (int i = 0; i < mScopeLength; i++)
    {
        mFIFOSampleArray.add(buffer.getSample(0, i % buffersize));
    }
}

void ScopeVisualizer::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xFF575659));
    g.fillRoundedRectangle(this->getLocalBounds().toFloat(), 4.f);
    //g.fillAll(juce::Colours::white);

    //code that draws response curve

    juce::Path p;

    for (int x = 0; x < getWidth(); x++) {
        float y_val = juce::jmap((float)x, 0.f, (float)getWidth(), 0.f, (float)mScopeLength);
        y_val = mFIFOSampleArray[(int)y_val];

        if (x == 0) {
            p.startNewSubPath(x, juce::jmap(y_val, -1.f, 1.f, (float)getHeight(), 0.f));
        }
        else {
            p.lineTo(x, juce::jmap(y_val, -1.f, 1.f, (float)getHeight(), 0.f));
        }
    }

    g.setColour(juce::Colour(0xFF59595B));
    g.strokePath(p, juce::PathStrokeType(1));
}

*/