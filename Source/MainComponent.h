#pragma once

#include <JuceHeader.h>
using namespace juce;

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

struct OwnedArrayComponent : Component
{
    OwnedArrayComponent()
    {
        for (int i = 0; i < 10; ++i) {
            auto* widget = widgets.add( new Widget(i) );
            addAndMakeVisible(widget);
        }
    }
    
    void resized() override
    {
        auto width = getWidth() / static_cast<float>(widgets.size());
        DBG("Width: " << width);
        int x = 0;
        auto h = getHeight();
        for (auto* widget : widgets) {
            widget->setBounds(x, 0, width, h);
            x += width;
        }

    }
    
    
    OwnedArray<Widget> widgets;
};

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
    OwnedArrayComponent ownedArrayComp;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
