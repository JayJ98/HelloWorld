#pragma once

#include <JuceHeader.h>
using namespace juce;
#include "DataRace.h"

//struct RepeatingThing;

//====================================================
struct ImageProcessingThread : Thread
{
    ImageProcessingThread(int w_, int h_);
    ~ImageProcessingThread();
    void run() override;
    
    void setUpdateRendererFunc( std::function<void(Image&&)> f );
    
private:
    int w {0};
    int h {0};
    std::function<void(Image&&)> updateRenderer;
    Random r;
    
};

//====================================================

struct LambdaTimer : Timer
{
    LambdaTimer(int ms, std::function<void()> f);
    ~LambdaTimer();
    void timerCallback() override;
private:
    std::function<void()> lambda;
};
#include <array>

struct Renderer : Component, AsyncUpdater
{
    Renderer();
    ~Renderer();
    void paint(Graphics& g) override;
    void handleAsyncUpdate() override;
private:
    std::unique_ptr<ImageProcessingThread> processingThread;
    std::unique_ptr<LambdaTimer> lamdaTimer;
    bool firstImage = true;
    std::array<Image, 2> imageToRender;
};

//====================================================



struct DualButton : public Component
{
    DualButton();
    void resized() override;
    
    void setButton1Handler(std::function<void()> f);
    void setButton2Handler(std::function<void()> f);
private:
//    RepeatingThing& timerThing;
    TextButton button1 {"button1"}, button2 {"button2"};
};

struct MyAsyncHighResGui : Component, AsyncUpdater, HighResolutionTimer
{
    void handleAsyncUpdate() override
    {
        paintColor = (paintColor + 1) % maxColors;
        repaint();
    }
    
    
    void hiResTimerCallback() override
    {
        triggerAsyncUpdate();
    }
    
    void paint(Graphics& g) override
    {
        switch (paintColor) {
            case 0:
                g.setColour(Colours::red);
                break;
            case 1:
                g.setColour(Colours::green);
                break;
            case 2:
                g.setColour(Colours::blue);
                break;
        }
        g.fillAll();
    }
    MyAsyncHighResGui()
    {
        this->startTimer(1000 / 5);
    }
    ~MyAsyncHighResGui()
    {
        stopTimer();
        cancelPendingUpdate();
    }
private:
    int paintColor = 0;
    const int maxColors{3};
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
    
    RepeatingThing() { startTimer(800); }
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
//        DBG("Main component mouse enter!");
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
    //    int counter = 0;
    MyComp comp;
    OwnedArrayComponent ownedArrayComp;
    RepeatingThing repeatingThing;
    DualButton dualButton; //{repeatingThing};
    MyAsyncHighResGui hiResGui;
//    Renderer renderer;
    
    Test test;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
