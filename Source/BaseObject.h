/*
  ==============================================================================

    BaseObject.h
    Created: 19 Feb 2017 10:30:52pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef BASEOBJECT_H_INCLUDED
#define BASEOBJECT_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
typedef Vector3D<double> Vector;
class RigidBody;
class MechanicalPoint
{
};
class BodyPoint
{
public:
    BodyPoint(double x, double y, double z=0);
    Vector bodyPoint;
};
class ForcePoint : public MechanicalPoint
{
    
};
class ImpulseEmitter : public ForcePoint
{
    
};
class ForceEmitter : public ForcePoint
{
    
};
class BaseObject
{
public:
    BaseObject();
    void setPosition(Vector p);
    virtual ~BaseObject(){}
protected:
    Vector originPoint;
    double yaw;
    virtual void draw(Graphics& g) = 0;
};



#endif  // BASEOBJECT_H_INCLUDED
