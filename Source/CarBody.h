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
    float ld =1.0f, sensorAngle = 0;
    float theta;
    float wheelAngle[4];
    bool setSensorLocation(Line<float> l)
    {
        Vector intersect;
        Vector s1 = Vector(-std::cos(theta),0,-std::sin(theta));
        Vector s2 = Vector(std::cos(theta),0,std::sin(theta));
        Vector left, right, center;
        center  = location + Vector(-std::sin(theta),0,std::cos(theta)) * 5;
        left = center + s1*3.5f;
        right = center + s2*3.5f;
        s1 = left;
        s2 = right;
        if(l.intersects(Line<float>(s1.x, s1.z, s2.x, s2.z)))
        {
            Point<float> p = l.getIntersection(Line<float>(s1.x, s1.z, s2.x, s2.z));
            intersect = Vector(p.x, 0, p.y);
            sensorAngle = std::atan((intersect-center).length()/(center-location).length());
            if((intersect-left).length()>(intersect-right).length())
                sensorAngle = -sensorAngle;
            ld = (intersect-location).length();
            //printf("%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf---\n",intersect.x, intersect.z, s1.x, s1.z, s2.x, s2.z);
            return true;
        }
        return false;
    }
    Vector getLocation()
    {
        return location;
    }
    Vector getDirection()
    {
        return Vector(-std::sin(theta), 0, std::cos(theta));
    }
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
        handleAngle = std::atan(2 * 2.7f * std::sin(sensorAngle)/ld);
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
    bool isCheck(float criterion)
    {
        if(dist>criterion)
        {
            dist = 0;
            return true;
        }
        return false;
    }
    CarState& getCarState();
    void forward(float val);
    void steer(float ang);
private:
    const float wheelHeight;
    const float frontWheelBase;
    const float rearWheelBase;
    const float frontWheelTrack;
    const float rearWheelTrack;
    float dist = 0;
    float velocity;
    float acceleration;
    
    float yawrate;
    float yawaccel;
    
    CarState carState;
};


#endif  // CARBODY_H_INCLUDED
