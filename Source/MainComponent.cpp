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

ImageProcessingThread::ImageProcessingThread(int w_, int h_, ImagePassingFunc f) : Thread("ImageProcessingThread"), w(w_), h(h_), updateRenderer( std::move(f) )
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
        
        for (int x = 0; x < w; ++x) {

            
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
        
        
        if( updateRenderer)
        {
            updateRenderer( canvas, *this);
        }
        wait( -1 );
    }
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
                                            getHeight(),
                                            [this](Image image,
                                                   ImageProcessingThread& thread)
                                               {

            imageToRender.push(image);

            if( !thread.threadShouldExit() )
            {
                lamdaTimer = std::make_unique<LambdaTimer>(1000, [this](){
                    processingThread->notify();
                });
            }
        });
    });
    
    startTimerHz(20);
}

Renderer::~Renderer()
{
    lamdaTimer.reset(nullptr);
    processingThread.reset();
}

void Renderer::paint(Graphics& g)
{
    g.drawImage(imageToRender.read(), getLocalBounds().toFloat());
}

void Renderer::timerCallback()
{
    repaint();
}
//=======================================================================================

Renderer2::Renderer2()
{
    Timer::callAfterDelay(10, [this]() 
                          {
        SafePointer<Renderer2> safePtr(this);
        if( safePtr.getComponent() )
            safePtr->loop();
        loop();
    });
}

void Renderer2::paint(Graphics& g)
{
    g.drawImage(imageToRender.read(), getLocalBounds().toFloat());
}

void Renderer2::loop()
{
    auto w = getWidth();
    auto h = getHeight();
    Thread::launch([w, h, this]()
    {
        Random r;
        auto canvas = Image(Image::PixelFormat::RGB, w, h, true);
        
        for (int x = 0; x < w; ++x) {

            
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
        SafePointer<Renderer2> safePtr(this);
        if( safePtr.getComponent() )
            safePtr-> imageToRender.push(canvas);
        
        Timer::callAfterDelay(10, [this]()
                              {
            SafePointer<Renderer2> safePtr(this);
            if( safePtr.getComponent() )
                safePtr->repaint();
        });
        Timer::callAfterDelay(1000, [this]()
                              {
            SafePointer<Renderer2> safePtr(this);
            if( safePtr.getComponent() )
                safePtr->loop();
        });
    });
}
//=======================================================================================


void DualButton::resized()
{
    auto bounds = getLocalBounds();
//    DBG(bounds.toString());
    bounds = bounds.removeFromLeft(100);
//    DBG(bounds.toString());
    button1.setBounds(bounds);

    bounds = getLocalBounds().removeFromRight(100);
    bounds.setX(bounds.getX() - 5);
    
//    DBG(bounds.toString());
    
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
    
    addAndMakeVisible(renderer);
    
    addAndMakeVisible(renderer2);
    
    setSize (800, 600);
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
    
    renderer.setBounds(hiResGui.getBounds().withX(hiResGui.getRight() + 5).withWidth(100) );
    
    renderer2.setBounds(renderer.getBounds().withX(renderer.getRight() + 5).withWidth(100) );
                             
}
