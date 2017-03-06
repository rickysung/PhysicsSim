/*
  ==============================================================================

    RigidBody.cpp
    Created: 21 Feb 2017 10:36:35pm
    Author:  Ricky

  ==============================================================================
*/

#include "RigidBody.h"

RigidBody::RigidBody(int w, int h)
{
    bodyPoints.add(BodyPoint(-w/2,-h/2, 0));
    bodyPoints.add(BodyPoint(-w/2,h/2, 0));
    bodyPoints.add(BodyPoint(w/2,h/2, 0));
    bodyPoints.add(BodyPoint(w/2,-h/2, 0));
}
void RigidBody::draw(Graphics& g)
{
    int i;
    for(i=bodyPoints.size();
        i-->0;
        g.drawLine(originPoint.x + bodyPoints.getUnchecked(i).bodyPoint.x,
                   originPoint.y + bodyPoints.getUnchecked(i).bodyPoint.y,
                   originPoint.x + bodyPoints.getUnchecked((i+1)%bodyPoints.size()).bodyPoint.x,
                   originPoint.y + bodyPoints.getUnchecked((i+1)%bodyPoints.size()).bodyPoint.y));
}