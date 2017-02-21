/*
  ==============================================================================

    RigidBody.h
    Created: 21 Feb 2017 10:36:35pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef RIGIDBODY_H_INCLUDED
#define RIGIDBODY_H_INCLUDED

#include "BaseObject.h"
class RigidBody : public BaseObject
{
public:
    RigidBody(int width, int height);
    double yaw;
    double pitch;
    double roll;
    Array<BodyPoint> bodyPoints;
public:
    void draw(Graphics& g) override;
};



#endif  // RIGIDBODY_H_INCLUDED
