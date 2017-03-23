/*
  ==============================================================================

    CarBody.cpp
    Created: 12 Mar 2017 10:02:37pm
    Author:  Ricky

  ==============================================================================
*/

#include "CarBody.h"
CarBody::CarBody(void (*ctlr)(CarBody&, CarState&), HandleAlgorithm* ha, Colour c, float wh, float fwb, float rwb, float fwt, float rwt) : bodyColour(c), wheelHeight(wh), frontWheelBase(fwb), rearWheelBase(rwb), frontWheelTrack(fwt), rearWheelTrack(rwt), controller(ctlr)
{
    handleAlgorithm = ha;
    if(ha!=nullptr)
    {
        ha->setCarBody(this);
    }
    carState.location = Vector(0,wheelHeight,0);
    velocity = 0;
    state_idx = 0;
    max_save_state = 0;
}
void CarBody::sensing(Array<Vector>& points)
{
    if(handleAlgorithm!=nullptr)
        handleAlgorithm->handleProcess(points);
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
    dist += velocity;
    controller(*this, this->getCarState());
}