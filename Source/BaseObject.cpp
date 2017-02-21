/*
  ==============================================================================

    BaseObject.cpp
    Created: 19 Feb 2017 10:30:52pm
    Author:  Ricky

  ==============================================================================
*/

#include "BaseObject.h"

BodyPoint::BodyPoint(double x, double y, double z)
{
    bodyPoint = Vector(x, y, z);
}
BaseObject::BaseObject() {}
void BaseObject::setPosition(Vector p)
{
    this->originPoint = p;
}