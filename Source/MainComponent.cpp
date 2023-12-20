#include "MainComponent.h"
using namespace juce;

OwnedArrayComponent::OwnedArrayComponent()
{
    for (int i = 0; i < 10; ++i) {
        auto* button = buttons.add( new TextButton(String(i) ));
        addAndMakeVisible(button);
        button->addListener(this);
    }
}

OwnedArrayComponent::~OwnedArrayComponent()
{
    for (auto* button : buttons) {
        button->removeListener(this);
    }
}
void OwnedArrayComponent::resized()
{
    auto width = getWidth() / static_cast<float>(buttons.size());
    DBG("Width: " << width);
    int x = 0;
    auto h = getHeight();
    for (auto* button : buttons) {
        button->setBounds(x, 0, width, h);
        x += width;
    }

}

void OwnedArrayComponent::buttonClicked(Button* buttonThatWasClicked)
{
    DBG("Something was clicked");
    if( buttonThatWasClicked == buttons.getFirst() )
    {
        DBG("The first button was clicked!");
    }
    else if( buttonThatWasClicked == buttons.getLast() )
    {
        DBG("The last button was clicked!");
    }
}
//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(comp);
//    comp.addMouseListener(this, false);
    
    addAndMakeVisible(ownedArrayComp);
    ownedArrayComp.addMouseListener(this, true);
    
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    comp.removeMouseListener(this);
}
//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (Font (36.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
    
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    comp.setBounds(30, 30, 100, 100);
    
    ownedArrayComp.setBounds(5,
                             comp.getBottom() + 5,
                             getWidth() -5,
                             getHeight() -  comp.getBottom() - 10);
    
}
