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
                OwnedArray<CarBody>& cb,
                      int screenWidth,
                   int screenHeight);
    void draw() override;
    void focusOn(int idx) { focusedCarBody = carBodys[idx]; }
    void BindTexture(GLuint textureID)
    {
        inputTexture = textureID;
    }
    float getOffsetX() { return offsetX; }
    float getOffsetZ() { return offsetZ; }
    void setOffsetX(float v) { offsetX = v; }
    void setOffsetZ(float v) { offsetZ = v; }
    float getTimer() { return timer; }
    bool getTrajectoryMode() { return isShowTrajectory; }
    void setTrajectoryMode(bool v) { isShowTrajectory = v; }
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
    float offsetX;
    float offsetZ;
    bool isShowTrajectory = false;
    void paintGL();
    Matrix getWheelMatrix(CarBody*, CarState, TIRE_INDEX);
    void drawCar(CarBody*, CarState);
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
    CarBody* focusedCarBody;
    OwnedArray<CarBody>& carBodys;
    ScopedPointer<OpenGLShaderProgram::Uniform> textureID;
};




#endif  // SCREENRENDERER_H_INCLUDED
