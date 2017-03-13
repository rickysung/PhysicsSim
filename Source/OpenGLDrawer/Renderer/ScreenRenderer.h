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
class ScreenRenderer : public SimRenderer
{
public:
    ScreenRenderer(OpenGLContext& glContext,
                   CarBody& carBody,
                      int screenWidth,
                   int screenHeight);
    void draw() override;
    void BindTexture(GLuint textureID)
    {
        inputTexture = textureID;
    }
    void saveState();
    float getTimer() { return timer; }
    
    Matrix getTransformMatrix() override;
    Matrix getInverseTransformMatrix() override;
    Matrix getViewMatrix() override;
    Matrix getProjectionMatrix() override;
    Matrix getInverseProjectionMatrix() override;
    Matrix getInverseViewMatrix() override;
private:
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
    CarBody& carBody;
    Array<CarState> carStates;
    int max_save_state;
    int state_idx;
    ScopedPointer<OpenGLShaderProgram::Uniform> textureID;
};




#endif  // SCREENRENDERER_H_INCLUDED
