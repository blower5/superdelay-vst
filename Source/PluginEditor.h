#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Graffix.h"
#include "XYPad.h"
#include "Visualizers.h"

//==============================================================================
/**
*/
class SuperdelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SuperdelayAudioProcessorEditor (SuperdelayAudioProcessor&);
    ~SuperdelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    Graffix mGraffix;

    SuperdelayAudioProcessor& audioProcessor;

    juce::Image mLogoImage;
    juce::Image mAuthorImage;

    juce::Slider mDryWetSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDryWetSliderAttachment;
    juce::Slider mTimeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mTimeSliderAttachment;
    juce::Slider mFeedbackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFeedbackSliderAttachment;

    juce::DrawableButton mTimeIsLockedButton{"timeislockedbutton",juce::DrawableButton::ButtonStyle::ImageRaw};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mTimeIsLockedButtonAttachment;
    juce::DrawableButton mFBInvertedButton{"fbinvertedbutton", juce::DrawableButton::ButtonStyle::ImageRaw};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mFBInvertedButtonAttachment;
    juce::DrawableButton mStereoButton{"stereobutton", juce::DrawableButton::ButtonStyle::ImageRaw};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mStereoButtonAttachment;

    std::unique_ptr<juce::ParameterAttachment> mXYXAttachment;
    std::unique_ptr<juce::ParameterAttachment> mXYYAttachment;
    XYPad mFilterXYPad;

    FilterVisualizer mFilterVisualizer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SuperdelayAudioProcessorEditor)
};
