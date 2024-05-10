#include "FilterDelay.h"

FilterDelay::FilterDelay()
{
    mSmoothedDryWet.setCurrentAndTargetValue(0.5f);
    mSmoothedFB.setCurrentAndTargetValue(0.5f);
    mSmoothedTime.setCurrentAndTargetValue(0.5f);
}

FilterDelay::~FilterDelay()
{
}

void FilterDelay::prepareToPlay(float inSampleRate, int samplesPerBlock)
{
    mSampleRate = inSampleRate;

    mDelayBuffer.setSize(2, mMaxDelayTimeSeconds * inSampleRate + 1);
    mDelayBuffer.clear();

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = inSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;

    mIIRLeft.prepare(spec);
    mIIRRight.prepare(spec);

    calculateIIR(.7, .7);

    mSmoothedDryWet.reset(inSampleRate, .01f);
    mSmoothedFB.reset(inSampleRate, .01f);
    mSmoothedTime.reset(inSampleRate, .15f);

    //mSmoothedTimeIsLocked.reset(inSampleRate, .01f);
    mSmoothedFBInverted.reset(inSampleRate, .01f);
    mSmoothedStereo.reset(inSampleRate, .01f);
}

void FilterDelay::processBlock(juce::AudioBuffer<float>& inBuffer)
{
    // setup our data
    int num_samples = inBuffer.getNumSamples();

    float* in_channel_left = inBuffer.getWritePointer(0);
    float* in_channel_right = inBuffer.getWritePointer(1);

    float* circular_channel_left = mDelayBuffer.getWritePointer(0);
    float* circular_channel_right = mDelayBuffer.getWritePointer(1);

    for (int i = 0; i < num_samples; i++) {

        int bufferlen = mDelayBuffer.getNumSamples();

        // get the input
        float fbinverted = mSmoothedFBInverted.getNextValue();

        float input_left = in_channel_left[i];
        float input_right = in_channel_right[i];

        in_channel_left[i] = 0.f;
        in_channel_right[i] = 0.f;

        //clear and write filtered delay line (fb inverted parameter gets scaled from -1 to 1)
        circular_channel_left[mDelayWritehead] = 0;
        circular_channel_right[mDelayWritehead] = 0;
        circular_channel_left[mDelayWritehead] = mIIRLeft.processSample(input_left + mFBLeft * (fbinverted * -2.f + 1.f));
        circular_channel_right[mDelayWritehead] = mIIRRight.processSample(input_right + mFBRight * (fbinverted * -2.f + 1.f));

        //hard limiter
        circular_channel_left[mDelayWritehead] = std::max(std::min(circular_channel_left[mDelayWritehead], 1.f), -1.f);
        circular_channel_right[mDelayWritehead] = std::max(std::min(circular_channel_right[mDelayWritehead], 1.f), -1.f);

        // calculate the delay time. minimium delay time is 0 samples
        int delay_time_samples = juce::jmax(mSmoothedTime.getNextValue() * mSampleRate, 0.f);
        int readhead = mDelayWritehead - delay_time_samples;

        // loop circular buffer (readhead can be negative!)
        readhead += bufferlen;
        readhead %= bufferlen;

        // pack the output
        float delay_out_left = circular_channel_left[readhead];
        float delay_out_right = circular_channel_right[readhead];

        // get the dry/wet / mix
        float wet_amount = mSmoothedDryWet.getNextValue();
        float dry_amount = 1.f - wet_amount;

        // generate / save the feedback for the next sample
        float fb_amount = mSmoothedFB.getNextValue();

        mFBLeft = delay_out_left * fb_amount;
        mFBRight = delay_out_right * fb_amount;

        //minus sign == snes magix
        in_channel_left[i] = (input_left * dry_amount) + (delay_out_left * wet_amount);
        in_channel_right[i] = (input_right * dry_amount) + (delay_out_right * wet_amount) * (mSmoothedStereo.getNextValue() * -2.f + 1.f);

        //manage writeheads
        mDelayWritehead++;
        mDelayWritehead %= bufferlen;
    }
}

void FilterDelay::calculateIIR(float inFilterFreq, float inFilterQ)
{
    float freq = freqFromNormalized(inFilterFreq);
    float Q = (powf(60, inFilterQ ) - .95f) / 50.f; //juce::jmap(inFilterQ,0.001f, 1.4f);

    mIIRLeft.coefficients = juce::dsp::IIR::Coefficients<float>::makeBandPass(mSampleRate, freq, Q);
    mIIRRight.coefficients = juce::dsp::IIR::Coefficients<float>::makeBandPass(mSampleRate, freq, Q);
}

double FilterDelay::getResponse(float inFreq)
{
    float freq = freqFromNormalized(inFreq);
    return (float)mIIRLeft.coefficients->getMagnitudeForFrequency(freq, mSampleRate);
}

juce::Array<float> FilterDelay::getBandValues()
{
    return juce::Array<float>(mLastFilterFreq - mLastFilterQ * .5, mLastFilterFreq + mLastFilterQ * .5);
}

void FilterDelay::setDelayParameters(float inDelayTimeSeconds, float inFBPercent, float inDryWetPercent, float inFBInverted, float inStereo)
{
    mSmoothedDryWet.setTargetValue(inDryWetPercent);
    mSmoothedFB.setTargetValue(inFBPercent);
    mSmoothedTime.setTargetValue(inDelayTimeSeconds);

    //mSmoothedTimeIsLocked.setTargetValue(inTimeIsLocked);
    mSmoothedFBInverted.setTargetValue(inFBInverted);
    mSmoothedStereo.setTargetValue(inStereo);
}

void FilterDelay::setFilterParameters(float inFilterFreq, float inFilterQ)
{
    //Calculate new IIR coefficients if parameters have changed
    if (inFilterFreq != mLastFilterFreq || inFilterQ != mLastFilterQ)
    {
        calculateIIR(inFilterFreq, inFilterQ);
    }
    mLastFilterFreq = inFilterFreq;
    mLastFilterQ = inFilterQ;
}

float FilterDelay::normalizeFreq(float inFreq)
{
    return logf(juce::jlimit(0.001f, 21950.f, inFreq) / 10.f) / logf(2200.f);
}

float FilterDelay::freqFromNormalized(float inNorm)
{
    return juce::jlimit(0.001f, 21950.f, 10 * powf(2200.f, inNorm));
}