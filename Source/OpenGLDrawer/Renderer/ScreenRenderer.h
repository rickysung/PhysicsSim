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

class ScreenRenderer : public SimRenderer
{
public:
    ScreenRenderer(OpenGLContext& glContext,
                      int screenWidth,
                      int screenHeight);
    void draw() override;
    void accel(float v);
    void stop();
    void BindTexture(GLuint textureID)
    {
        inputTexture = textureID;
    }
    float getTimer() { return timer; }
    float getVelocity() { return velocity; }
    float getAcceleration() {return acceleration; }
    float getLocation() { return trans; }
    Matrix getTransformMatrix() override;
    Matrix getInverseTransformMatrix() override;
    Matrix getViewMatrix() override;
    Matrix getProjectionMatrix() override;
    Matrix getInverseProjectionMatrix() override;
    Matrix getInverseViewMatrix() override;
private:
    void drawCar();
    float timer = 0;
    float acceleration = 0;
    float velocity = 0;
    float trans = 0;
    GLuint inputTexture;
    virtual const char* getVertexShader() override;
    virtual const char* getFragmentShader() override;
    ScreenShape screenShape;
    SphereGridShape gridShape;
    FloorShape floorShape;
    ObjShape carShape;
    ObjShape wheelShape;
    ScopedPointer<OpenGLShaderProgram::Uniform> textureID;
};




#endif  // SCREENRENDERER_H_INCLUDED
