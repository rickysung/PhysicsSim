/*
  ==============================================================================

    CarBody.h
    Created: 12 Mar 2017 10:02:37pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef CARBODY_H_INCLUDED
#define CARBODY_H_INCLUDED

#include "RigidBody.h"
struct CarState
{
    float handleAngle;
    Vector location;
    float theta;
    float wheelAngle[4];
    
    Matrix getLocationMatrix()
    {
        float cr = std::cos(theta);
        float sr = std::sin(theta);
        return Matrix(cr, 0, sr, 0,
                      0, 1, 0, 0,
                      -sr, 0, cr, 0,
                      location.x, location.y, location.z, 1);
    }
    Matrix getInverseLocationMatrix()
    {
        float cr = std::cos(-theta);
        float sr = std::sin(-theta);
        return Matrix(1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      -location.x, -location.y, -location.z, 1) *
        Matrix(cr, 0, sr, 0,
               0, 1, 0, 0,
               -sr, 0, cr, 0,
               0,0,0,1);
    }
    void steer(float val)
    {
        handleAngle += val;
    }
    Matrix getWheelMatrix(float wheelTrack, float wheelBase, int wi, bool isHandle)
    {
        Matrix t;
        Matrix r;
        float cw, sw;
        float cr, sr;
        float sign = wheelTrack>0?1:-1;
        cw = std::cos(wheelAngle[wi]);
        sw = std::sin(wheelAngle[wi]);
        cr = std::cos(isHandle?handleAngle:0);
        sr = std::sin(isHandle?handleAngle:0);
        t = Matrix(sign * cr,0,sign * sr,0,
                   -sw*sr,cw,sw*cr,0,
                   -cw*sr,-sw,cw*cr,0,
                   wheelTrack,0,wheelBase,1);
        return t;
    }
    void progress(float vx, float vz, float wheelVelocity, float yawrate)
    {
        int i;
        for(i=0 ; i<4 ; i++)
            wheelAngle[i] += wheelVelocity;
        location.x += vx;
        location.z += vz;
        theta += yawrate;
    }
private:
};
class CarBody : public BaseObject
{
public:
    CarBody(float wheelHeight, float frontWheelBase, float rearWheelBase, float frontWheel, float rearWheel);
    void progress();
    float getWheelHeight() { return wheelHeight; }
    float getFrontWheelBase() { return frontWheelBase; }
    float getRearWheelBase() { return rearWheelBase; }
    float getFrontWheelTrack() { return frontWheelTrack; }
    float getRearWheelTrack() { return rearWheelTrack; }
//    Matrix getLocationMatrix();
//    Matrix getInverseLocationMatrix();
//    Matrix getWheelMatrix(TIRE_INDEX idx);
    CarState& getCarState();
    void forward(float val);
    void steer(float ang);
private:
    const float wheelHeight;
    const float frontWheelBase;
    const float rearWheelBase;
    const float frontWheelTrack;
    const float rearWheelTrack;
    
    float velocity;
    float acceleration;
    
    float yawrate;
    float yawaccel;
    
    CarState carState;
};


#endif  // CARBODY_H_INCLUDED
