#include "MainComponent.h"
using namespace juce;


DualButton::DualButton()
{
    addAndMakeVisible(button1);
    addAndMakeVisible(button2);
    
//    button1.onClick = [this]()
//    {
//        timerThing.stopTimer();
//        DBG( "Button1's size: " << this->button1.getBounds().toString() );
//    };
//    
//    button2.onClick = [this]()
//    {
//        timerThing.startTimer(10);
//        DBG( "Button2's size: " << this->button2.getBounds().toString() );
//    };
}

ImageProcessingThread::ImageProcessingThread(int w_, int h_) : Thread("ImageProcessingThread"), w(w_), h(h_)
{
    startThread();
}
ImageProcessingThread::~ImageProcessingThread()
{
    stopThread(500);
}
void ImageProcessingThread::run()
{
    while( true )
    {
        if( threadShouldExit() ) { break; }
        
        auto canvas = Image(Image::PixelFormat::RGB, w, h, true);
        
        if( threadShouldExit() ) { break; }
        
        if( threadShouldExit() ) { break; }
        
        DBG("[ImageProcessingThread] generating random image " << Time::getCurrentTime().toISO8601(true));
        
        bool shouldBail = false;
        for (int x = 0; x < w; ++x) {
            if( threadShouldExit() )
            {
                shouldBail = true;
                break;
            }
            for (int y = 0; y < h; ++y)
            {
                canvas.setPixelAt(x,
                                  y,
                                  Colour(
                                         r.nextInt(),
                                         r.nextFloat(),
                                         r.nextFloat(),
                                         1.f ) );
            }
        }
        
        if( threadShouldExit() || shouldBail) { break; }
        
        if( updateRenderer)
        {
            updateRenderer( std::move(canvas) );
        }
        wait( -1 );
    }
}

void ImageProcessingThread::setUpdateRendererFunc( std::function<void(Image&&)> f )
{
    updateRenderer = std::move(f);
}

//=======================================================================================

LambdaTimer::LambdaTimer(int ms, std::function<void()> f) : lambda( std::move(f) )
{
    startTimer(ms);
}

LambdaTimer::~LambdaTimer()
{
    stopTimer();
}

void LambdaTimer::timerCallback()
{
    stopTimer();
    if( lambda )
        lambda();
}

Renderer::Renderer()
{
    lamdaTimer = std::make_unique<LambdaTimer>(10, [this]()
    {
        processingThread = std::make_unique<ImageProcessingThread>(getWidth(),
                                                    getHeight());
        processingThread->setUpdateRendererFunc([this](Image&& image)
                                               {
            int renderIndex = firstImage ? 0 : 1;
            firstImage = !firstImage;
            imageToRender[renderIndex] = std::move(image);
            
            triggerAsyncUpdate();
            lamdaTimer = std::make_unique<LambdaTimer>(1000, [this](){
                processingThread->notify();
            });
        });
    });
}

Renderer::~Renderer()
{
    processingThread.reset();
    lamdaTimer.reset();
}

void Renderer::paint(Graphics& g)
{
    DBG("[Renderer] painting: " << Time::getCurrentTime().toISO8601(true) << "\n");
    g.drawImage(firstImage ? imageToRender[0] : imageToRender[1], getLocalBounds().toFloat());
}

void Renderer::handleAsyncUpdate()
{
    repaint();
}
//=======================================================================================


void DualButton::resized()
{
    auto bounds = getLocalBounds();
    DBG(bounds.toString());
    bounds = bounds.removeFromLeft(100);
    DBG(bounds.toString());
    button1.setBounds(bounds);

    bounds = getLocalBounds().removeFromRight(100);
    bounds.setX(bounds.getX() - 5);
    
    DBG(bounds.toString());
    
    button2.setBounds(bounds);
}

void DualButton::setButton1Handler(std::function<void()> f)
{
    button1.onClick = f;
}

void DualButton::setButton2Handler(std::function<void()> f)
{
    button2.onClick = f;
}//==============================================================================

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
    auto width = ( (getWidth() - (buttons.size() * 3)) / static_cast<float>(buttons.size()) );
    int x = 0;
    auto h = getHeight();
    for (auto* button : buttons) {
        button->setBounds(x, 0, width, h);
        x += width + 3;
    }

}

void OwnedArrayComponent::buttonClicked(Button* buttonThatWasClicked)
{
    DBG("Button " << buttonThatWasClicked->getButtonText() << " was clicked!");  
}
//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(comp);
//    comp.addMouseListener(this, false);
    
    addAndMakeVisible(ownedArrayComp);
    ownedArrayComp.addMouseListener(this, true);
    addAndMakeVisible(dualButton);
    
    dualButton.setButton1Handler(
        [this]()
         {
             repeatingThing.startTimerHz(2);
//             DBG( "Button1's size: " << dualButton.button1.getBounds().toString() );
         }
                                 );
    dualButton.setButton2Handler(
        [this]()
         {
             repeatingThing.startTimerHz(8);
//             DBG( "Button1's size: " << dualButton.button1.getBounds().toString() );
         }
                                 );
    
    addAndMakeVisible(repeatingThing);
    addAndMakeVisible(hiResGui);
    
//    addAndMakeVisible(renderer);
    
    setSize (800, 400);
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
//    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
    
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
    
    dualButton.setBounds(comp.getBounds().withX(comp.getRight()+5).withWidth(210));

    repeatingThing.setBounds(dualButton.getBounds().withX(dualButton.getRight() + 5).withWidth(100) );
    
    hiResGui.setBounds(repeatingThing.getBounds().withX(repeatingThing.getRight() + 5).withWidth(100) );
    
//    renderer.setBounds(hiResGui.getBounds().withX(hiResGui.getRight() + 5).withWidth(100) );
                             
}
