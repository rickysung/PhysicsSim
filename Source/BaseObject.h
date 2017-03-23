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




#endif  // BASEOBJECT_H_INCLUDED
