/*
  ==============================================================================

    NodeRenderer.h
    Created: 2 Jan 2017 9:30:33pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef NODERENDERER_H_INCLUDED
#define NODERENDERER_H_INCLUDED


#include "../../OpenGLDrawer.h"

class SimRenderer : public OpenGLDrawer
{
public:
    SimRenderer(OpenGLContext& glContext,
                  int screenWidth,
                  int screenHeight) :
    OpenGLDrawer(glContext, screenWidth, screenHeight, 1),
    flatObjectShape(glContext),
    objectShape(glContext),
    channelShape(glContext),
    ambiShape(glContext),
    controllerShape(glContext),
    labelShape(glContext)
    {
        genFrameBuffer();
    }
    void initializeUniformVariables()
    {
        modelScale = createUniform("modelScale");
        modelMatrix = createUniform("modelMatrix");
        viewMatrix = createUniform("viewMatrix");
        projectionMatrix = createUniform("projectionMatrix");
        bodyColour = createUniform("bodyColour");
        idColour = createUniform("idColour");
        nodeType = createUniform("nodeType");
        isUniformColour = createUniform("isUniformColour");
        modelMode = createUniform("modelMode");
        lineWidth = createUniform("lineWidth");
        nodeIndex = createUniform("nodeIndex");
        nodeNum = createUniform("nodeNum");
        labelTexture = createUniform("labelTexture");
        ambientLoc = createUniform("material.ambient");
        diffuseLoc = createUniform("material.diffuse");
        specularLoc = createUniform("material.specular");
        shineLoc = createUniform("material.shininess");
        lightAmbientLoc = createUniform("light.ambient");
        lightDiffuseLoc = createUniform("light.diffuse");
        lightSpecularLoc = createUniform("light.specular");
        isMaterialMode = createUniform("isMaterialMode");
    }
    void BindLabelTexture(GLuint inputTexture)
    {
        labelTextureID = inputTexture;
    }
    virtual Matrix getTransformMatrix() = 0;
    virtual Matrix getInverseTransformMatrix() = 0;
    virtual Matrix getViewMatrix() = 0;
    virtual Matrix getProjectionMatrix() = 0;
    virtual Matrix getInverseProjectionMatrix() = 0;
    virtual Matrix getInverseViewMatrix() = 0;
    
    void setMousePosition(int x, int y) { mx = x, my= y, isSelectMode=true; }
    virtual float getCameraAzimuth() { return camAzimuth; }
    virtual float getCameraElevation() { return camElevation; }
    virtual float getCameraDistance() { return camDistance; }
    virtual float getCameraYaw() { return camYaw; }
    virtual float getCameraPitch() { return camPitch; }
    virtual void setCameraAzimuth(float val) { camAzimuth = val; }
    virtual void setCameraElevation(float val) { camElevation = val; }
    virtual void setCameraDistance(float val) { camDistance = val; }
    virtual void setCameraYaw(float val) { camYaw = val; }
    virtual void setCameraPitch(float val) { camPitch = val; }
    float getViewAngleHorizon() { return viewAngleHorizon; }
    float getViewAngleVertical() { return viewAngleVertical; }
    void setViewAngleHorizon(float val) { viewAngleHorizon = val; }
    void setViewAngleVertical(float val) { viewAngleVertical = val; }
    
protected:
    float viewAngleHorizon;
    float viewAngleVertical;
    float camAzimuth;
    float camElevation;
    float camDistance;
    float camYaw;
    float camPitch;
    float nodeSize;
    
    ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix;
    ScopedPointer<OpenGLShaderProgram::Uniform> viewMatrix;
    ScopedPointer<OpenGLShaderProgram::Uniform> modelMatrix;
    ScopedPointer<OpenGLShaderProgram::Uniform> bodyColour;
    ScopedPointer<OpenGLShaderProgram::Uniform> idColour;
    ScopedPointer<OpenGLShaderProgram::Uniform> isUniformColour;
    ScopedPointer<OpenGLShaderProgram::Uniform> nodeType;
    ScopedPointer<OpenGLShaderProgram::Uniform> modelScale;
    ScopedPointer<OpenGLShaderProgram::Uniform> lineWidth;
    ScopedPointer<OpenGLShaderProgram::Uniform> modelMode;
    ScopedPointer<OpenGLShaderProgram::Uniform> nodeIndex;
    ScopedPointer<OpenGLShaderProgram::Uniform> nodeNum;
    ScopedPointer<OpenGLShaderProgram::Uniform> labelTexture;
    ScopedPointer<OpenGLShaderProgram::Uniform> ambientLoc;
    ScopedPointer<OpenGLShaderProgram::Uniform> diffuseLoc;
    ScopedPointer<OpenGLShaderProgram::Uniform> specularLoc;
    ScopedPointer<OpenGLShaderProgram::Uniform> shineLoc;
    ScopedPointer<OpenGLShaderProgram::Uniform> lightAmbientLoc;
    ScopedPointer<OpenGLShaderProgram::Uniform> lightDiffuseLoc;
    ScopedPointer<OpenGLShaderProgram::Uniform> lightSpecularLoc;
    ScopedPointer<OpenGLShaderProgram::Uniform> isMaterialMode;
    //    virtual Matrix getModelMatrixAtPosition(Matrix baseMatrix, float ns, float azi, float elv, float dis) = 0;
 
    void drawShape(Shape& shapeToDraw,
                   Colour iColour,
                   Colour bColour = Colour(0x00000000))
    {
        if(bColour != Colour(0x00000000))
        {
            idColour->set(iColour.getFloatRed(), iColour.getFloatGreen(), iColour.getFloatBlue(), iColour.getFloatAlpha());
            bodyColour->set(bColour.getFloatRed(), bColour.getFloatGreen(), bColour.getFloatBlue(), bColour.getFloatAlpha());
            isUniformColour->set(1);
        }
        
        shapeToDraw.draw();
        idColour->set(0.0f,0.0f,0.0f,0.0f);
        nodeType->set(0);
        isUniformColour->set(0);
    }
    
    GLuint labelTextureID;
    FlatObjectShape flatObjectShape;
    ObjectShape objectShape;
    SpeakerShape channelShape;
    AmbisonicShape ambiShape;
    AmbisonicControllerShape controllerShape;
    LabelShape labelShape;
    GLubyte *data;
    int mx= 0, my = 0;
    bool isSelectMode = false;
};


#endif  // NODEVIEWRENDERER_H_INCLUDED
