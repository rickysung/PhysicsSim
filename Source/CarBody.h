/*
  ==============================================================================

    CarBody.h
    Created: 12 Mar 2017 10:02:37pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef CARBODY_H_INCLUDED
#define CARBODY_H_INCLUDED
#include "BaseObject.h"
#include "HandleAlgorithm.h"
struct CarState
{
    float handleAngle = 0;
    Vector location;
    float theta = 0;
    float yawRate = 0;
    float wheelAngle[4] = {0,};
    
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
    void progress(float vx, float vz, float yr, float wheelVelocity)
    {
        int i;
        location.x += vx;
        location.z += vz;
        yawRate = yr;
        theta += yawRate*0.01f;
        for(i=0 ; i<4 ; i++)
            wheelAngle[i] += wheelVelocity;
    }
private:
};

class CarBody
{
public:
    CarBody(void (*cm)(CarBody&, CarState&), HandleAlgorithm* ha, Colour carBodyColour, float wheelHeight, float frontWheelBase, float rearWheelBase, float frontWheel, float rearWheel);
    void progress();
    float getVelocity() { return velocity; }
    float getLateralVelocity() { return lateralVelocity; }
    void setLateralVelocity(float val) { lateralVelocity = val; }
    float getWheelHeight() { return wheelHeight; }
    float getFrontWheelBase() { return frontWheelBase; }
    float getRearWheelBase() { return rearWheelBase; }
    float getFrontWheelTrack() { return frontWheelTrack; }
    float getRearWheelTrack() { return rearWheelTrack; }
    bool stateCheck(float criterion)
    {
        if(dist>criterion && max_save_state>0)
        {
            saveState();
            dist = 0;
            return true;
        }
        return false;
    }
    Colour getBodyColour() { return bodyColour; }
    Array<CarState> getStateHistory(){ return stateHistory; }
    CarState& getCarState();
    void forward(float val);
    void steer(float ang);
    void sensing(Array<Vector>&);
private:
    ScopedPointer<HandleAlgorithm> handleAlgorithm;
    Colour bodyColour;
    Array<CarState> stateHistory;
    void saveState();
    const float wheelHeight;
    const float frontWheelBase;
    const float rearWheelBase;
    const float frontWheelTrack;
    const float rearWheelTrack;
    void (*controller)(CarBody&, CarState&);
    float dist = 0;
    float lateralVelocity;
    float velocity;
    int max_save_state;
    int state_idx;
    CarState carState;
};
class centerPursuitMethod : public HandleAlgorithm
{
public:
    centerPursuitMethod(float p) : pGain(p){}
    void setCarBody(CarBody* cb) override
    {
        carBody = cb;
        sensorDistance = 4.0f;
        sensorWidth = 3.0f;
        centerLocation = sensorWidth;
    }
private:
    void getIntersectPoints(Line<float> l, Array<float>& points)
    {
        CarState& carState = carBody->getCarState();
        Vector s1 = Vector(-std::cos(carState.theta), 0, -std::sin(carState.theta));
        Vector s2 = Vector( std::cos(carState.theta), 0,  std::sin(carState.theta));
        Vector left, right, center, intersect;
        center  = carState.location + Vector(-std::sin(carState.theta),0,std::cos(carState.theta)) * sensorDistance;
        left = center + s1*sensorWidth;
        right = center + s2*sensorWidth;
        float dist;
        Line<float> sensorLine = Line<float>(left.x, left.z, right.x, right.z);
        if(l.intersects(sensorLine))
        {
            Point<float> p = l.getIntersection(sensorLine);
            intersect = Vector(p.x, 0, p.y);
            dist = (intersect-left).length();
            points.add(dist);
            if(dist>centerLocation)
                return;
        }
    }
    void handleProcess(Array<Vector>& points) override
    {
        int i, n;
        float left = 0, right = 2*sensorWidth;
        Array<float> crossPoints;
        if(carBody==nullptr)
            return;
        CarState& carState = carBody->getCarState();
        n = points.size();
        for(i=0 ; i<n-1 ; i+=2)
        {
            getIntersectPoints(Line<float>(points[i].x,
                                           points[i].z,
                                           points[i+1].x,
                                           points[i+1].z), crossPoints);
        }
        n = crossPoints.size();
        for(i=0 ; i<n ; i++)
        {
            if(crossPoints[i]<centerLocation)
            {
                left = crossPoints[i];
            }
            else
                right = crossPoints[i];
        }
        centerLocation = (left + right)/2;
        targetAngle = pGain * (sensorWidth - centerLocation);
        if(std::abs(targetAngle-carState.handleAngle)<0.1)
            carState.handleAngle = targetAngle;
        else
            carState.handleAngle = carState.handleAngle + (targetAngle>carState.handleAngle?0.1:-0.1);
        if(carState.handleAngle>M_PI/3)
            carState.handleAngle = M_PI/3;
        else if(carState.handleAngle<-M_PI/3)
            carState.handleAngle = -M_PI/3;
    }
    float targetAngle;
    float centerLocation;
    float sensorDistance;
    float sensorWidth;
    float pGain;
};
class purePursuitMethod : public HandleAlgorithm
{
public:
    purePursuitMethod(float p)
    {
        pGain = p;
    }
    void setCarBody(CarBody* cb) override
    {
        carBody = cb;
        ld = carBody->getRearWheelBase() + carBody->getFrontWheelBase();
        sensorAngle = 0;
        sensorDistance = 4.5f;
        sensorWidth = 2.5f;
    }
private:
    bool setSensorLocation(Line<float> l)
    {
        CarState& carState = carBody->getCarState();
        Vector intersect;
        Vector s1 = Vector(-std::cos(carState.theta), 0, -std::sin(carState.theta));
        Vector s2 = Vector( std::cos(carState.theta), 0,  std::sin(carState.theta));
        Vector left, right, center;
        center  = carState.location + Vector(-std::sin(carState.theta),0,std::cos(carState.theta)) * sensorDistance;
        left = center + s1*sensorWidth;
        right = center + s2*sensorWidth;
        s1 = left;
        s2 = right;
        if(l.intersects(Line<float>(s1.x, s1.z, s2.x, s2.z)))
        {
            Point<float> p = l.getIntersection(Line<float>(s1.x, s1.z, s2.x, s2.z));
            intersect = Vector(p.x, 0, p.y);
            sensorAngle = std::atan((intersect-center).length()/(center-carState.location).length());
            if((intersect-left).length()>(intersect-right).length())
                sensorAngle = -sensorAngle;
            ld = (intersect-carState.location).length();
            return true;
        }
        return false;
    }
    void handleProcess(Array<Vector>& points) override
    {
        if(carBody==nullptr)
            return;
        float carLength = carBody->getFrontWheelBase()+carBody->getRearWheelBase();
        CarState& carState = carBody->getCarState();
        int i, n;
        n = points.size();
        bool isIntersact = false;
        for(i=0 ; i<n-1 ; i+=2)
        {
            if(!isIntersact)
            {
                isIntersact = setSensorLocation(Line<float>(points[i].x,
                                                points[i].z,
                                                points[i+1].x,
                                                points[i+1].z));
            }
            else
                break;
        }
        carState.handleAngle = std::atan(2 * pGain * carLength * std::sin(sensorAngle)/ld);
    }
    float pGain = 1.0f;
    float ld = 1.0f;
    float sensorAngle = 0;
    float sensorDistance = 0;
    float sensorWidth = 0;
};
class ManualMethod : public HandleAlgorithm
{
public:
    void setCarBody(CarBody* cb) override
    {
        carBody = cb;
    }
    void steer(float a)
    {
        ha += a;
    }
private:
    void handleProcess(Array<Vector>& points) override
    {
        CarState& carState = carBody->getCarState();
        carState.handleAngle = ha;
    }
    float ha = 0;
};
#endif  // CARBODY_H_INCLUDED
