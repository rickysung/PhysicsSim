/*
  ==============================================================================

    HandleAlgorithm.h
    Created: 23 Mar 2017 2:24:51pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef HANDLEALGORITHM_H_INCLUDED
#define HANDLEALGORITHM_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"
typedef Vector3D<float> Vector;
class CarBody;
class HandleAlgorithm{
    friend class CarBody;
public:
    virtual void setCarBody(CarBody* c) =0;
    virtual ~HandleAlgorithm(){}
protected:
    CarBody* carBody;
    virtual void handleProcess(Array<Vector>&)=0;
    int algorithmCounter=0;
};



#endif  // HANDLEALGORITHM_H_INCLUDED
