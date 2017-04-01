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
void kinematicMovement(CarBody& carBody, CarState& carState)
{
    float dt = 0.01f;
    float rearWheelBase = carBody.getRearWheelBase();
    float frontWheelBase = carBody.getFrontWheelBase();
    float wheelHeight = carBody.getWheelHeight();
    float velocity = carBody.getVelocity();
    float yawrate = carState.yawRate;
    float radius = carState.handleAngle==0?0:((frontWheelBase + rearWheelBase)/std::tan(carState.handleAngle));
    float vx;
    float vz;
    float d = carState.handleAngle;
    float t = carState.theta;
    yawrate = radius==0?0:(velocity/radius);
    vx = -velocity * std::sin(t) - rearWheelBase * yawrate * std::cos(t+d);
    vz = velocity * std::cos(t) - rearWheelBase * yawrate * std::sin(t+d);
    vx *= dt;
    vz *= dt;
    carState.progress(vx, vz, yawrate*dt, velocity/wheelHeight*dt);
}
void dynamicMovement(CarBody& carBody, CarState& carState)
{
    float dt = 0.01f;
    float lr = carBody.getRearWheelBase();
    float lf = carBody.getFrontWheelBase();
    float wheelHeight = carBody.getWheelHeight();
    float vx = carBody.getVelocity();
    float r = carState.yawRate;
    float vy = carBody.getLateralVelocity();
    float d = carState.handleAngle;
    float m= 1573;
    float iz = 2873;
    float cf = 80000, cr = 80000;
    float dvy, dr;
    float t, gx, gz;
    t = carState.theta;
    if(vx!=0)
    {
        dvy = -2*(cf+cr)/(m*vx)*vy + (2*(lr*cr-lf*cf)/(m*vx)-vx)*r + 2*cf*d/m;
        dr = 2*(lr*cr-lf*cf)/(iz*vx)*vy - 2*(lf*lf*cf+lr*lr*cr)*r/(iz*vx) + 2*lf*cf*d/iz;
        carBody.setLateralVelocity(vy + dvy*dt);
        gx = -vx * std::sin(t) - lr * r * std::cos(t+d) + dvy * std::sin(t+d) * dt*0.5f;
        gz =  vx * std::cos(t) - lr * r * std::sin(t+d) + dvy * std::cos(t+d) * dt*0.5f;
        gx *= dt;
        gz *= dt;
        carState.progress(gx, gz, (r + dr*dt*0.5f), vx/wheelHeight*dt);
    }
}

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
    setWantsKeyboardFocus(true);
    handleMethod = new ManualMethod();
//    addCarBody(kinematicMovement, new centerPursuitMethod(0.8f), Colours::crimson,
//               0.31f, 1.14f, 1.56f, 0.851f, 0.85f);
//    addCarBody(kinematicMovement, new purePursuitMethod(1.0f), Colours::burlywood,
//               0.31f, 1.14f, 1.56f, 0.851f, 0.85f);
    addCarBody(dynamicMovement, new centerPursuitMethod(0.8f), Colours::coral,
               0.31f, 1.14f, 1.56f, 0.851f, 0.85f);
   // addCarBody(Colours::green, 0.31f, 1.14f, 1.56f, 0.851f, 0.85f);
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
//    Array<Vector>& points = carRenderer->getRoadPoints();
//    Vector carLoc = carBody->getCarState().getLocation();
//    Vector carDir = carBody->getCarState().getDirection();
//    Vector p;
//    int n = points.size();
//    bool isIntersact = false;
//    int i;
//    float mag =5.0f;
//    float cx = getWidth()/2;
//    float cy = getHeight()/2;
//    g.setColour(Colours::white);
//    for(i=0 ; i<n-1 ; i+=2)
//    {
//        g.setColour(Colours::white);
//        g.drawLine(cx + points[i].x*mag, cy - points[i].z*mag,cx + points[i+1].x*mag, cy - points[i+1].z*mag);
//    }
//    g.setColour(Colours::red);
//    g.drawEllipse(cx + carLoc.x*mag-1, cy - carLoc.z*mag-1, 2, 2, 1);
//    g.setColour(Colours::red);
//    g.drawEllipse(cx + (carLoc.x+1.5*carDir.x)*mag, cy - (carLoc.z+carDir.z*1.5)*mag, 2, 2, 1);
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
        startAzi = carRenderer->getCameraAzimuth();
        startElv = carRenderer->getCameraElevation();
    }
    else
    {
        startYaw = carRenderer->getCameraYaw();
        startPch = carRenderer->getCameraPitch();
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
        carRenderer->setCameraAzimuth(startAzi - mx);
        carRenderer->setCameraElevation(startElv + my);
    }
    else
    {
        carRenderer->setCameraYaw(startYaw + mx);
        carRenderer->setCameraPitch(startPch + my);
    }
    //[/UserCode_mouseDrag]
}

void PhysicsContainer::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel)
{
    //[UserCode_mouseWheelMove] -- Add your code here...
    float dis = carRenderer->getCameraDistance();
    float diff = -wheel.deltaY;
    carRenderer->setCameraDistance(dis + diff);
    //[/UserCode_mouseWheelMove]
}

bool PhysicsContainer::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
    int i;
    CarBody* carBody;
    if(key==KeyPress::downKey)
    {
        for(i=0 ; i<carBodys.size() ; i++)
        {
            carBody = carBodys.getUnchecked(i);
            carBody->forward(-1);
        }
        return true;
    }
    else if(key==KeyPress::upKey)
    {
        if(!isTimerRunning())
        {
            startTimer(10);
        }
        for(i=0 ; i<carBodys.size() ; i++)
        {
            carBody = carBodys.getUnchecked(i);
            carBody->forward(1);
        }
        return true;
    }
//    else if(key==KeyPress::spaceKey)
//    {
//        stopTimer();
//        return true;
//    }
    else if(key.getKeyCode()=='1')
    {
        focusedCarIndex--;
        if(focusedCarIndex<0)
        {
            focusedCarIndex = carBodys.size()-1;
        }
        return true;
    }
    else if(key.getKeyCode()=='2')
    {
        focusedCarIndex++;
        if(focusedCarIndex>=carBodys.size())
        {
            focusedCarIndex = 0;
        }
        return true;
    }
    else if(key==KeyPress::leftKey)
    {
        handleMethod->steer(0.05);
    }
    else if(key==KeyPress::rightKey)
    {
        handleMethod->steer(-0.05);
    }
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PhysicsContainer::addCarBody(void (*ctrl)(CarBody&, CarState&), HandleAlgorithm* ha, Colour bc, float wheelHeight, float frontWheelBase, float rearWheelBase, float frontWheelTrack, float rearWheelTrack)
{
    
    CarBody* carBody = new CarBody(ctrl, ha, bc, wheelHeight,
                          frontWheelBase,
                          rearWheelBase,
                          frontWheelTrack,
                                   rearWheelTrack);
    carBodys.add(carBody);
}
void PhysicsContainer::timerCallback()
{
    int i;
    CarBody* carBody;
    for(i=0 ; i<carBodys.size() ; i++)
    {
        carBody = carBodys.getUnchecked(i);
        carBody->stateCheck(7.0f);
    }
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
    carRenderer = new CarRenderer(openGLContext, carBodys, getWidth(), getHeight());
}
void PhysicsContainer::render()
{
    int i;
    int n;
    CarBody* carBody;
    Vector p;
    n = carBodys.size();
    carRenderer->focusOn(focusedCarIndex);
    for(i=0 ; i<n ; i++)
    {
        carBody = carBodys[i];
        carBody->sensing(carRenderer->getRoadPoints());
        carBody->progress();
    }
    carRenderer->draw();
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
