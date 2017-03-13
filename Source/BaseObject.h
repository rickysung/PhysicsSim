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
typedef Vector3D<float> Vector;
typedef Matrix3D<float> Matrix;

class RigidBody;
enum class TIRE_INDEX{
    FRONT_LEFT = 0,
    FRONT_RIGHT,
    REAR_LEFT,
    REAR_RIGHT
};
class MechanicalPoint
{
};
class BodyPoint
{
public:
    BodyPoint(float, float, float);
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
};



#endif  // BASEOBJECT_H_INCLUDED
