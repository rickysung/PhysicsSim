/*
  ==============================================================================

    PanoramaVideoRenderer.h
    Created: 21 Aug 2016 3:07:40pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef PANORAMAVIDEORENDERER_H_INCLUDED
#define PANORAMAVIDEORENDERER_H_INCLUDED

#include "PanoramicNodeRenderer.h"

class PanoramaVideoRenderer : public PanoramicNodeRenderer
{
public:
    PanoramaVideoRenderer(OpenGLContext& glContext,
                    int screenWidth,
                    int screenHeight,
                    DataModel& nodelist);
    void draw() override;
    Matrix getViewMatrix(bool isInterative = true) override;
    Matrix getViewMatrix2();
    Matrix getProjectionMatrix() override;
    Matrix getInverseProjectionMatrix() override;
    Matrix getInverseViewMatrix(bool isInterative = true) override;
    
    Matrix getTransformMatrix() override;
    Matrix getInverseTransformMatrix() override;
    float getListenerYaw() override { return NodeList.getListenerYaw(); };
    void setListenerYaw(float val) override { NodeList.setListenerYaw(val); };
    void BindTexture(GLuint textureID)
    {
        inputTexture = textureID;
    }
    void paint();
    virtual const char* getVertexShader() override;
    virtual const char* getFragmentShader() override;
    GLuint inputTexture;
    GlobeShape globeShape;
    SphereGridShape gridShape;
    ScopedPointer<OpenGLShaderProgram::Uniform> videoTexture;
    ScopedPointer<OpenGLShaderProgram::Uniform> stereoScopicMode;
    ScopedPointer<OpenGLShaderProgram::Uniform> isVideoMode;
};




#endif  // PANORAMAVIDEORENDERER_H_INCLUDED
