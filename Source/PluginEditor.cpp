#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuperdelayAudioProcessorEditor::SuperdelayAudioProcessorEditor(SuperdelayAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), mFilterVisualizer(&p)
{
    setResizable(false, false);
    //465, 337 original
    setSize(765, 337);

    setLookAndFeel(&mGraffix);

    //=========================================================================
    mDryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    mDryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    addAndMakeVisible(mDryWetSlider);

    mDryWetSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),
        "dry_wet",
        mDryWetSlider));

    mTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    mTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    addAndMakeVisible(mTimeSlider);

    mTimeSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),
        (*audioProcessor.getVTS().getRawParameterValue("timeislocked"))
        ?
        "timesnes"
        :
        "time",
        mTimeSlider));

    mFeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    mFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    addAndMakeVisible(mFeedbackSlider);

    mFeedbackSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),
        "fb",
        mFeedbackSlider));

    //=================================================================================================================

    const juce::DrawableImage timeislockedbuttonimage = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::timeislockedbutton_png, BinaryData::timeislockedbutton_pngSize));
    const juce::DrawableImage timeislockedbuttonimagedown = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::timeislockedbuttondown_png, BinaryData::timeislockedbuttondown_pngSize));
    const juce::DrawableImage timeislockedbuttonimageon = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::timeislockedbuttonon_png, BinaryData::timeislockedbuttonon_pngSize));

    const juce::DrawableImage fbinvertedbuttonimage = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::fbinvertedbutton_png, BinaryData::fbinvertedbutton_pngSize));
    const juce::DrawableImage fbinvertedbuttonimagedown = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::fbinvertedbuttondown_png, BinaryData::fbinvertedbuttondown_pngSize));
    const juce::DrawableImage fbinvertedbuttonimageon = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::fbinvertedbuttonon_png, BinaryData::fbinvertedbuttonon_pngSize));

    const juce::DrawableImage stereobuttonimage = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::stereobutton_png, BinaryData::stereobutton_pngSize));
    const juce::DrawableImage stereobuttonimagedown = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::stereobuttondown_png, BinaryData::stereobuttondown_pngSize));
    const juce::DrawableImage stereobuttonimageon = juce::DrawableImage(juce::ImageFileFormat::loadFrom(BinaryData::stereobuttonon_png, BinaryData::stereobuttonon_pngSize));

    mLogoImage = juce::ImageFileFormat::loadFrom(BinaryData::logo_png, BinaryData::logo_pngSize);
    mAuthorImage = juce::ImageFileFormat::loadFrom(BinaryData::author_png, BinaryData::author_pngSize);

    mTimeIsLockedButton.setImages(
        &timeislockedbuttonimage,
        nullptr,
        &timeislockedbuttonimagedown,
        nullptr,
        &timeislockedbuttonimageon,
        nullptr,
        &timeislockedbuttonimagedown,
        nullptr
    );
    mTimeIsLockedButton.setToggleable(true);
    mTimeIsLockedButton.setClickingTogglesState(true);
    addAndMakeVisible(mTimeIsLockedButton);
    mTimeIsLockedButtonAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.getVTS(),
        "timeislocked",
        mTimeIsLockedButton));

    //TIME LOCKING BEHAVIOR
    mTimeIsLockedButton.onStateChange = [this]()
    {
        if (mTimeIsLockedButton.getToggleState())
        {
            mTimeSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(), "timesnes", mTimeSlider));
        }
        else
        {
            mTimeSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(), "time", mTimeSlider));
        }
        mTimeSlider.repaint();
    };

    mFBInvertedButton.setImages(
        &fbinvertedbuttonimage,
        nullptr,
        &fbinvertedbuttonimagedown,
        nullptr,
        &fbinvertedbuttonimageon,
        nullptr,
        &fbinvertedbuttonimagedown,
        nullptr
    );

    mFBInvertedButton.setToggleable(true);
    mFBInvertedButton.setClickingTogglesState(true);
    addAndMakeVisible(mFBInvertedButton);
    mFBInvertedButtonAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.getVTS(),
        "fbinv",
        mFBInvertedButton));

    mStereoButton.setImages(
        &stereobuttonimage,
        nullptr,
        &stereobuttonimagedown,
        nullptr,
        &stereobuttonimageon,
        nullptr,
        &stereobuttonimagedown,
        nullptr
    );
    mStereoButton.setToggleable(true);
    mStereoButton.setClickingTogglesState(true);
    addAndMakeVisible(mStereoButton);
    mStereoButtonAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.getVTS(),
        "stereo",
        mStereoButton));

    //=================================================================================================================

    addAndMakeVisible(mFilterVisualizer);

    mFilterXYPad.setOnHandleDragged([this]() {
        audioProcessor.getVTS().getParameter("filterfreq")->setValueNotifyingHost(mFilterXYPad.getXValue());
        audioProcessor.getVTS().getParameter("filterband")->setValueNotifyingHost(mFilterXYPad.getYValue());
        });

    mXYXAttachment.reset(new juce::ParameterAttachment(*audioProcessor.getVTS().getParameter("filterfreq"),
        [this](float inParameterValue) {
            mFilterXYPad.setXValue(audioProcessor.getVTS().getParameter("filterfreq")->getValue());
        }));
    mXYYAttachment.reset(new juce::ParameterAttachment(*audioProcessor.getVTS().getParameter("filterband"),
        [this](float inParameterValue) {
            mFilterXYPad.setYValue(audioProcessor.getVTS().getParameter("filterband")->getValue());
        }));

    addAndMakeVisible(mFilterXYPad);

    mXYXAttachment->sendInitialUpdate();
    mXYYAttachment->sendInitialUpdate();
}

SuperdelayAudioProcessorEditor::~SuperdelayAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void SuperdelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(mGraffix.getUIColour(Graffix::UIColour::main1));
    g.drawImageAt(mLogoImage, 12, 12);
    g.drawImageAt(mAuthorImage, 10, 314);

    g.setFont(juce::Font("Century Gothic", 20.f, juce::Font::bold));
    g.setColour(mGraffix.getUIColour(Graffix::UIColour::main3));

    g.drawFittedText("MIX",         49,  125,  34,  15,  juce::Justification::centred, 1);
    g.drawFittedText("DELAY TIME",  166, 154,  96,  14,  juce::Justification::centred, 1);
    g.drawFittedText("FEEDBACK",    343, 154,  87,  14,  juce::Justification::centred, 1);

}

void SuperdelayAudioProcessorEditor::resized()
{
    // taken from mock up image
    float width = getWidth();
    float height = getHeight();
    mDryWetSlider.setBounds(36, 145, 61, 79);;
    mTimeSlider.setBounds(144, 171, 141, 159);
    mFeedbackSlider.setBounds(316, 171, 141, 159);

    mTimeIsLockedButton.setBounds(160, 116, 108, 29);
    mFBInvertedButton.setBounds(332, 116, 108, 29);
    mStereoButton.setBounds(12, 267, 108, 29);

    mFilterXYPad.setBounds(478, 116, 274, 190);

    mFilterVisualizer.setBounds(478, 116, 274, 190);

}
