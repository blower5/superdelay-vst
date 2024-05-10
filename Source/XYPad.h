#pragma once

#include <JuceHeader.h>

class XYHandle : public juce::Component
{
public:
    
    /* */
    void paint(juce::Graphics& g) override;
    
    /* */
    void mouseEnter (const juce::MouseEvent& e) override;
    
    /* */
    void mouseExit (const juce::MouseEvent& e) override;
    
    /* */
    void mouseDown (const juce::MouseEvent& e) override;

    void mouseUp(const juce::MouseEvent& e) override;

    /* */
    void mouseDrag (const juce::MouseEvent& e) override;

    /* */
    std::function<void()> onHandleDragged = nullptr;
    
private:
    
    juce::ComponentDragger mDragger;
};

class XYPad : public juce::Component
{
public:

    static constexpr int mHandleSize = 30;
    XYPad();
    void paint(juce::Graphics& g) override;
    void resized() override;
    //void mouseDown(const juce::MouseEvent& e) override;

    void setXValue(float inXValue)
    {
        mHandle.setCentrePosition(juce::jmap(inXValue, 0.f, (float)getWidth()), mHandle.getBounds().getCentreY());
    }

    void setYValue(float inYValue)
    {
        mHandle.setCentrePosition(mHandle.getBounds().getCentreX(), juce::jmap(inYValue, (float)getHeight(), 0.f));
    }

    float getXValue() {
        return juce::jmap((float)mHandle.getBounds().getX() + (float)mHandle.getBounds().getWidth() / 2, 0.f, (float)getWidth(), 0.f, 1.f);
    }
    
    float getYValue() {
        return juce::jmap((float)mHandle.getBounds().getY() + (float)mHandle.getBounds().getHeight() / 2, 0.f, (float)getHeight(), 1.f, 0.f);
    }
    
    void setOnHandleDragged(std::function<void()> inFunction) {
        mHandle.onHandleDragged = inFunction;
    }
    
private:
    
    juce::ComponentDragger mPadDragger;
    XYHandle mHandle;
};
