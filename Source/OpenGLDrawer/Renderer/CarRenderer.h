/*
  ==============================================================================

    ScreenRenderer.h
    Created: 20 Aug 2016 8:19:03pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef SCREENRENDERER_H_INCLUDED
#define SCREENRENDERER_H_INCLUDED
#include "../OpenGLDrawer.h"
#include "BaseRenderer/NodeRenderer.h"
#include "../../CarBody.h"
class CarRenderer : public SimRenderer
{
public:
    CarRenderer(OpenGLContext& glContext,
                      int screenWidth,
                   int screenHeight);
    void draw() override;
    void BindTexture(GLuint textureID)
    {
        inputTexture = textureID;
    }
    void saveState();
    float getTimer() { return timer; }
    
    void setCarBody(CarBody*);
    Matrix getTransformMatrix() override;
    Matrix getInverseTransformMatrix() override;
    Matrix getViewMatrix() override;
    Matrix getViewMatrix(float, float, float, float, float);
    Matrix getProjectionMatrix() override;
    Matrix getProjectionMatrix(float, float);
    Matrix getInverseProjectionMatrix() override;
    Matrix getInverseViewMatrix() override;
    Array<Vector>& getRoadPoints() {return roadShape.getRoadPoints();}
private:
    void paintGL();
    Matrix getWheelMatrix(CarState, TIRE_INDEX);
    void drawCar(CarState);
    float timer = 0;
    GLuint inputTexture;
    virtual const char* getVertexShader() override;
    virtual const char* getFragmentShader() override;
    ScreenShape screenShape;
    SphereGridShape gridShape;
    FloorShape floorShape;
    ObjShape carShape;
    ObjShape wheelShape;
    RoadShape roadShape;
    ScopedPointer<CarBody> carBody;
    Array<CarState> carStates;
    int max_save_state;
    int state_idx;
    ScopedPointer<OpenGLShaderProgram::Uniform> textureID;
};




#endif  // SCREENRENDERER_H_INCLUDED
