/*
  ==============================================================================

    CarBody.cpp
    Created: 12 Mar 2017 10:02:37pm
    Author:  Ricky

  ==============================================================================
*/

#include "CarBody.h"
CarBody::CarBody(float wh, float fwb, float rwb, float fwt, float rwt) : wheelHeight(wh), frontWheelBase(fwb), rearWheelBase(rwb), frontWheelTrack(fwt), rearWheelTrack(rwt)
{
    carState.location = Vector(0,wheelHeight,0);
    yawrate = 0;
    yawaccel = 0;
    acceleration = 0;
    velocity = 0;
    carState.handleAngle = 0;
}
//Matrix CarBody::getLocationMatrix()
//{
//    return carState.getLocationMatrix();
//}
//Matrix CarBody::getInverseLocationMatrix()
//{
//    return carState.getInverseLocationMatrix();
//}
//Matrix CarBody::getWheelMatrix()
//{
//    switch (idx) {
//        case TIRE_INDEX::FRONT_LEFT:
//            return carState.getWheelMatrix(-frontWheelTrack, frontWheelBase, static_cast<int>(idx), true);
//            break;
//        case TIRE_INDEX::FRONT_RIGHT:
//            return carState.getWheelMatrix(frontWheelTrack, frontWheelBase, static_cast<int>(idx), true);
//            break;
//        case TIRE_INDEX::REAR_LEFT:
//            return carState.getWheelMatrix(-rearWheelTrack, -rearWheelBase, static_cast<int>(idx), false);
//            break;
//        case TIRE_INDEX::REAR_RIGHT:
//            return carState.getWheelMatrix(rearWheelTrack, -rearWheelBase, static_cast<int>(idx), false);
//            break;
//        default:
//            break;
//    }
//}
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
    float d = carState.handleAngle;
    float t = carState.theta;
    yawrate = radius==0?0:(velocity/radius);
    vx = -velocity * std::sin(t) - rearWheelBase * yawrate * std::cos(t+d);
    vz = velocity * std::cos(t) - rearWheelBase * yawrate * std::sin(t+d);
    carState.progress(vx, vz, velocity/wheelHeight, yawrate);
}