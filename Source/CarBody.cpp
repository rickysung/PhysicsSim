/*
  ==============================================================================

    CarBody.cpp
    Created: 12 Mar 2017 10:02:37pm
    Author:  Ricky

  ==============================================================================
*/

#include "CarBody.h"
CarBody::CarBody(Colour c, float wh, float fwb, float rwb, float fwt, float rwt) : bodyColour(c), wheelHeight(wh), frontWheelBase(fwb), rearWheelBase(rwb), frontWheelTrack(fwt), rearWheelTrack(rwt)
{
    carState.location = Vector(0,wheelHeight,0);
    yawrate = 0;
    yawaccel = 0;
    acceleration = 0;
    velocity = 0;
    carState.handleAngle = 0;
    state_idx = 0;
    max_save_state = 0;
    sensorAngle = 0;
    ld = frontWheelBase+rearWheelBase;
}
bool CarBody::setSensorLocation(Line<float> l)
{
    Vector intersect;
    Vector s1 = Vector(-std::cos(carState.theta),0,-std::sin(carState.theta));
    Vector s2 = Vector(std::cos(carState.theta),0,std::sin(carState.theta));
    Vector left, right, center;
    center  = carState.location + Vector(-std::sin(carState.theta),0,std::cos(carState.theta)) * 4.5f;
    left = center + s1*2.5f;
    right = center + s2*2.5f;
    s1 = left;
    s2 = right;
    if(l.intersects(Line<float>(s1.x, s1.z, s2.x, s2.z)))
    {
        Point<float> p = l.getIntersection(Line<float>(s1.x, s1.z, s2.x, s2.z));
        intersect = Vector(p.x, 0, p.y);
        this->sensorAngle = std::atan((intersect-center).length()/(center-carState.location).length());
        if((intersect-left).length()>(intersect-right).length())
            sensorAngle = -sensorAngle;
        this->ld = (intersect-carState.location).length();
        return true;
    }
    return false;
}
void CarBody::sensing(Array<Vector>& points)
{
    int i, n;
    n = points.size();
    bool isIntersact = false;
    for(i=0 ; i<n-1 ; i+=2)
    {
        if(!isIntersact)
        {
            isIntersact
            = setSensorLocation(Line<float>(points[i].x,
                                                     points[i].z,
                                                     points[i+1].x,
                                                     points[i+1].z));
        }
        else
            break;
    }
}
void CarBody::saveState()
{
    if(stateHistory.size()==max_save_state)
    {
        stateHistory.set(state_idx,carState);
        state_idx=(++state_idx)%max_save_state;
    }
    else
    {
        stateHistory.add(carState);
    }
}

CarState& CarBody::getCarState()
{
    return carState;
}
void CarBody::forward(float val)
{
    velocity += val;
}
void CarBody::steer(float val)
{
    carState.steer(val);
}
void CarBody::progress()
{
    float radius = carState.handleAngle==0?0:((frontWheelBase + rearWheelBase)/std::tan(carState.handleAngle));
    float vx;
    float vz;
    float handleAngle;
    float d = carState.handleAngle;
    float t = carState.theta;
    yawrate = radius==0?0:(velocity/radius);
    vx = -velocity * std::sin(t) - rearWheelBase * yawrate * std::cos(t+d);
    vz = velocity * std::cos(t) - rearWheelBase * yawrate * std::sin(t+d);
    handleAngle = std::atan(2 * (frontWheelBase+rearWheelBase) * std::sin(sensorAngle)/ld);
    carState.progress(handleAngle, vx, vz, yawrate, velocity/wheelHeight);
    dist += velocity;
}