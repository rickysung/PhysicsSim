/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.3.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "PhysicsContainer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PhysicsContainer::PhysicsContainer ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    carBody = new CarBody(0.31f,
                          1.16f,
                          1.54f,
                          0.851f,
                          0.85f);
    setWantsKeyboardFocus(true);

    //[/Constructor]
}

PhysicsContainer::~PhysicsContainer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    shutdownOpenGL();
    //[/Destructor]
}

//==============================================================================
void PhysicsContainer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //int n = Bodys.size();
    g.setColour(Colours::white);
//    drawGraph(g, "accel", 10, 10, 200, 100, t.size(), t.getRawDataPointer());
//    drawGraph(g, "accel", 10, 120, 200, 100, v.size(), v.getRawDataPointer());
//    drawGraph(g, "accel", 10, 230, 200, 100, a.size(), a.getRawDataPointer());
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
//    RigidBody* body;
//    for(int i=0 ; i<n ; i++)
//    {
//        body = dynamic_cast<RigidBody*>(Bodys.getUnchecked(i));
//        body->draw(g);
//    }
    //[/UserPaint]
}

void PhysicsContainer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PhysicsContainer::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
    if(!e.mods.isCommandDown())
    {
        startAzi = screenRenderer->getCameraAzimuth();
        startElv = screenRenderer->getCameraElevation();
    }
    else
    {
        startYaw = screenRenderer->getCameraYaw();
        startPch = screenRenderer->getCameraPitch();
    }
    //[/UserCode_mouseDown]
}

void PhysicsContainer::mouseDrag (const MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    float mx, my;
    mx = e.getDistanceFromDragStartX() * 0.01f;
    my = e.getDistanceFromDragStartY() * 0.01f;
    if(!e.mods.isCommandDown())
    {
        screenRenderer->setCameraAzimuth(startAzi - mx);
        screenRenderer->setCameraElevation(startElv + my);
    }
    else
    {
        screenRenderer->setCameraYaw(startYaw + mx);
        screenRenderer->setCameraPitch(startPch + my);
    }
    //[/UserCode_mouseDrag]
}

void PhysicsContainer::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel)
{
    //[UserCode_mouseWheelMove] -- Add your code here...
    float dis = screenRenderer->getCameraDistance();
    float diff = -wheel.deltaY;
    screenRenderer->setCameraDistance(dis + diff);
    //[/UserCode_mouseWheelMove]
}

bool PhysicsContainer::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...

    if(key==KeyPress::downKey)
    {
        carBody->forward(-0.01);
        return true;
    }
    else if(key==KeyPress::upKey)
    {
        if(!isTimerRunning())
        {
            startTimer(1000);
        }
        carBody->forward(0.01);
        return true;
    }
    else if(key==KeyPress::spaceKey)
    {
        stopTimer();
        return true;
    }
    else if(key==KeyPress::leftKey)
    {
        carBody->steer(0.05);
        return true;
    }
    else if(key==KeyPress::rightKey)
    {
        carBody->steer(-0.05);
        return true;
    }
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PhysicsContainer::timerCallback()
{
    screenRenderer->saveState();
}
void PhysicsContainer::drawGraph(Graphics& g, String name, int x, int y, int w, int h, int num, float values[])
{
    g.setColour(Colours::white);
    g.fillRect(x, y, w, h);
    int i;
    int stride = num/w>1?num/w:1;
    float min = 99999999;
    float max = -99999999;
    for(i=0 ; i<num ; i++)
    {
        if(min>values[i])
            min = values[i];
        else if(max<values[i])
            max = values[i];
    }
    if(num>stride*3)
    {
        if(max == min)
        {
            max+=0.0001f;
        }
        g.setColour(Colours::red);
        for(i=0 ; i<num-stride ; i+=stride)
        {
            g.drawLine(x + i*w/num,
                       y + h - h * (values[i]-min)/(max-min),
                       x + (i+stride)*w/num,
                       y + h - h * (values[i+stride]-min)/(max-min)
                       );
        }
    }
}
void PhysicsContainer::initialise()
{
    screenRenderer = new ScreenRenderer(openGLContext, *carBody, getWidth(), getHeight());
}
void PhysicsContainer::render()
{
//    if(screenRenderer->getAcceleration()!=0)
//    {
//        v.add(screenRenderer->getVelocity());
//        a.add(screenRenderer->getAcceleration());
//        t.add(screenRenderer->getLocation());
//    }
    carBody->progress();
    screenRenderer->draw();
    repaint();
}
void PhysicsContainer::shutdown()
{

}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PhysicsContainer" componentName=""
                 parentClasses="public OpenGLAppComponent, public Timer" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="mouseDrag (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseWheelMove (const MouseEvent&amp; e, const MouseWheelDetails&amp; wheel)"/>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ffffff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
