#pragma once

#include <JuceHeader.h>

using namespace juce;

struct MyComp : Component
{
//    void resized() override {}
    void paint(Graphics& g) override { g.fillAll(juce::Colours::darkorange); }
    void mouseEnter(const MouseEvent& e) override
    {
        DBG("MyComp Mouse enter!");
    }
    void mouseExit(const MouseEvent& e) override
    {
        DBG("MyComp Mouse exit!");
    }
    
    void mouseDown(const MouseEvent& e) override
    {
        DBG("MyComp Mouse down!");
    }
    
    void mouseUp(const MouseEvent& e) override
    {
        DBG("MyComp Mouse up!");
    }
    
    void mouseDrag(const MouseEvent& e) override
    {
        DBG("MyComp Mouse drag at x: " << e.x << " y: " << e.y);
    }
    void mouseMove(const MouseEvent& e) override
    {
        DBG("MyComp mouse moved: " << counter);
        counter++;
    }
private:
    int counter = 0;
};
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseEnter(const MouseEvent& e) override
    {
        DBG("Main component mouse enter!");
    }
    
    void mouseExit(const MouseEvent& e) override
    {
        DBG("Main component mouse exit!");
    }
    
    void mouseMove(const MouseEvent& e) override
    {
        DBG("MainComponent mouse moved: " << counter);
        counter += 100;
    }
private:
    //==============================================================================
    // Your private member variables go here...
    
    MyComp comp;
    int counter = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
