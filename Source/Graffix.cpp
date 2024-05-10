#include "Graffix.h"

Graffix::Graffix()
{
}

Graffix::~Graffix()
{
}

juce::Colour Graffix::getUIColour(UIColour index) const noexcept
{
    return juce::Colour(mpallette[index]);
}


void Graffix::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)width * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // gradient fill
    g.setGradientFill(  juce::ColourGradient(getUIColour(UIColour::accent1), centreX, ry, getUIColour(UIColour::accent1low), centreX, ry + radius*2, false)  );
    g.fillEllipse(rx, ry, rw, rw);

    juce::Path p;
    auto pointerThickness = 12.0f;
    p.addEllipse(-radius * .1, -radius * .9, radius*.25, radius * .25);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(getUIColour(UIColour::main1));
    g.fillPath(p);
}

juce::Font Graffix::getLabelFont(juce::Label& label) 
{
#if JUCE_WINDOWS
    return juce::Font("Century Gothic", 20.f, juce::Font::bold);
#elif JUCE_MAC
    return juce::Font("Helvetica", 20.f, juce::font::bold);
#endif
}

void Graffix::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    if (!label.isBeingEdited())
    {
        float alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font(getLabelFont(label));

        g.setColour(getUIColour(UIColour::main3).withMultipliedAlpha(alpha));
        g.setFont(font);

        juce::Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
            juce::jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
            label.getMinimumHorizontalScale());

        g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour(label.findColour(juce::Label::outlineColourId));
    }
    //outline
    //g.drawRect(label.getLocalBounds());
}