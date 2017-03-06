/*
  ==============================================================================

    360VideoRenderer.h
    Created: 20 Aug 2016 8:17:50pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef VIDEO360RENDERER_H_INCLUDED
#define VIDEO360RENDERER_H_INCLUDED
#include "BaseRenderer/SpatialNodeRenderer.h"

class GlobeVideoRenderer : public SpatialNodeRenderer
{
public:
    GlobeVideoRenderer(OpenGLContext& glContext,
                    int screenWidth,
                    int screenHeight,
                    DataModel& nodelist);
    void draw() override;
    Matrix getViewMatrix(bool isInterative = true) override;
    Matrix getProjectionMatrix() override;
    Matrix getInverseProjectionMatrix() override;
    Matrix getInverseViewMatrix(bool isInterative = true) override;
    
    Matrix getTransformMatrix() override;
    Matrix getInverseTransformMatrix() override;
    
    
    
    float getGlobeRadius() { return globeRadius; }
    void BindTexture(GLuint textureID)
    {
        inputTexture = textureID;
    }
    float getListenerYaw() override { return NodeList.getListenerYaw(); };
    void setListenerYaw(float val) override { NodeList.setListenerYaw(val); };
    float getListenerPitch() override { return NodeList.getListenerPitch(); };
    void setListenerPitch(float val) override { NodeList.setListenerPitch(val); };
private:
    virtual const char* getVertexShader() override;
    virtual const char* getFragmentShader() override;
    float globeRadius;
    GLuint inputTexture;
    GlobeShape globeShape;
    SphereGridShape gridShape;
    ScopedPointer<OpenGLShaderProgram::Uniform> videoTexture;
    ScopedPointer<OpenGLShaderProgram::Uniform> stereoScopicMode;
    ScopedPointer<OpenGLShaderProgram::Uniform> isVideoMode;
    
};



#endif  // 360VIDEORENDERER_H_INCLUDED
