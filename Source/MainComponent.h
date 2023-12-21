#pragma once

#include <JuceHeader.h>
using namespace juce;

struct DualButton : public Component
{
    DualButton();
    void resized() override;
private:
    TextButton button1 {"button1"}, button2 {"button2"};
};

struct Widget : public Component
{
    Widget(int i) : num(i) {}
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::red);
        g.setColour(Colours::black);
        g.drawRect(getLocalBounds().reduced(2));
        
        g.drawFittedText(String(num), getLocalBounds(), Justification::centred, 1);
    }
    int num = 0;
};

//==============================================================================

struct RepeatingThing : Component, Timer
{
    void timerCallback()
    {
        drawRed = !drawRed;
        repaint();
    }
    void paint(Graphics& g)
    {
        g.fillAll( drawRed ? Colours::red : Colours::green );
    }
    
    RepeatingThing() { startTimerHz(2); }
    ~RepeatingThing() { stopTimer(); }
    
private:
    bool drawRed = true;
};
//==============================================================================

struct OwnedArrayComponent : Component, Button::Listener
{
    OwnedArrayComponent();
    ~OwnedArrayComponent();
    void resized() override;
    void buttonClicked(Button* buttonThatWasClicked) override;
private:
    OwnedArray<TextButton> buttons;
};

struct MyComp : Component
{
//    void resized() override {}
    void paint(Graphics& g) override { g.fillAll(juce::Colours::darkorange); }
    void mouseEnter(const MouseEvent& e) override
    {
//        DBG("MyComp Mouse enter!");
    }
    void mouseExit(const MouseEvent& e) override
    {
//        DBG("MyComp Mouse exit!");
    }
    
    void mouseDown(const MouseEvent& e) override
    {
//        DBG("MyComp Mouse down!");
    }
    
    void mouseUp(const MouseEvent& e) override
    {
//        DBG("MyComp Mouse up!");
    }
    
    void mouseDrag(const MouseEvent& e) override
    {
//        DBG("MyComp Mouse drag at x: " << e.x << " y: " << e.y);
    }
    void mouseMove(const MouseEvent& e) override
    {
//        DBG("MyComp mouse moved: " << counter);
//        counter++;
    }
private:
//    int counter = 0;
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
        /*DBG*/("Main component mouse enter!");
    }
    
    void mouseExit(const MouseEvent& e) override
    {
//        DBG("Main component mouse exit!");
    }
    
    void mouseMove(const MouseEvent& e) override
    {
//        DBG("MainComponent mouse moved: " << counter);
//        counter += 1;
    }
private:
    //==============================================================================
    // Your private member variables go here...
    DualButton dualButton;
    MyComp comp;
    int counter = 0;
    OwnedArrayComponent ownedArrayComp;
    RepeatingThing repeatingThing;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
