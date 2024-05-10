#include "XYPad.h"

//== HANDLE
void XYHandle::paint(juce::Graphics& g)
{
    //g.setColour(juce::Colour(0xFFAAA7DE));
    
    juce::Rectangle<float> bounds = getLocalBounds().toFloat();
    bounds.expand(-5, -5);
   // bounds.translate(bounds.getX() * .33, bounds.getY() * .33);
    g.setGradientFill(juce::ColourGradient(juce::Colour(0xFFAAA7DE), 0.f, bounds.getCentreY() - 5.f, juce::Colour(0xFF9895CF), 0.f, bounds.getCentreY() + 5.f, false));
    g.fillEllipse(bounds);
}

/* */
void XYHandle::mouseEnter(const juce::MouseEvent& e)
{
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
}

/* */
void XYHandle::mouseExit(const juce::MouseEvent& e)
{
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

/* */
void XYHandle::mouseDown(const juce::MouseEvent& e)
{
    mDragger.startDraggingComponent(this, e);
    setMouseCursor(juce::MouseCursor::NoCursor);
}

void XYHandle::mouseUp(const juce::MouseEvent& e)
{
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
}

/* */
void XYHandle::mouseDrag(const juce::MouseEvent& e)
{
    mDragger.dragComponent(this, e, nullptr);

    auto parent_component = getParentComponent();
    //constrain to pad
    auto x = juce::jlimit(0, parent_component->getWidth(), getBounds().getCentreX());
    auto y = juce::jlimit(0, parent_component->getHeight(), getBounds().getCentreY());
    setCentrePosition(x, y);
    
    juce::NullCheckedInvocation::invoke (onHandleDragged);
    
    parent_component->repaint();
}

//== PAD

XYPad::XYPad()
{
    addAndMakeVisible(mHandle);
}

void XYPad::paint(juce::Graphics& g)
{
}

void XYPad::resized()
{
    mHandle.setBounds(getWidth()/2 - mHandleSize/2, getHeight()/2 - mHandleSize/2, mHandleSize, mHandleSize);
}