/*
  ==============================================================================

    ObjectRenderer.h
    Created: 20 Aug 2016 8:12:41pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef OBJECTRENDERER_H_INCLUDED
#define OBJECTRENDERER_H_INCLUDED

#include "BaseRenderer/SpatialNodeRenderer.h"
#include "PlugInProcessor.h"

class ObjectRenderer : public SpatialNodeRenderer
{
public:
    ObjectRenderer(OpenGLContext& glContext,
                                     int screenWidth,
                                     int screenHeight,
                                     DataModel& nodelist,
                                     GworksMasterAudioProcessor& p);
    Matrix getTransformMatrix() override;
    Matrix getInverseTransformMatrix() override;
    Matrix getViewMatrix(bool isInterative = true) override;
    Matrix getProjectionMatrix() override;
    Matrix getInverseProjectionMatrix() override;
    Matrix getInverseViewMatrix(bool isInterative = true) override;
    void draw() override;
    
    float getListenerYaw() override
    {
//        if(prevFlag!=NodeList.isSyncView)
//        {
//            if(prevFlag)
//            {
//                camAzimuth = NodeList.getListenerYaw();
//            }
//            prevFlag = NodeList.isSyncView;
//        }
//        if(NodeList.isSyncView)
//            return NodeList.getListenerYaw();
        return camAzimuth;
    };
    float getListenerPitch() override
    {
        return camElevation;
    }
    void setListenerYaw(float val) override
    {
//        if(NodeList.isSyncView)
//            NodeList.setListenerYaw(val);
//        else
            camAzimuth = val;
    };
    void setListenerPitch(float val) override
    {
        camElevation = val;
    }
    void setViewPortMode(bool mode)
    {
        if(NodeList.isSyncView)
        {
            camAzimuth = NodeList.getListenerYaw();
        }
    }
    GworksMasterAudioProcessor& processor;

private:
    const char* getVertexShader() override;
    const char* getFragmentShader() override;
    PolarGridShape gridShape;
    CursorShape cursorShape;
    HeadShape headShape;
    HeadOrientShape headOrientShape;
    bool prevFlag;
    ScopedPointer<OpenGLShaderProgram::Uniform> isLight;
    ScopedPointer<OpenGLShaderProgram::Uniform> gainNum;
    ScopedPointer<OpenGLShaderProgram::Uniform> gains;
};



#endif  // OBJECTRENDERER_H_INCLUDED
