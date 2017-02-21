/*
  ==============================================================================

    GaussianBlurRenderer.h
    Created: 20 Aug 2016 8:43:12pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef GAUSSIANBLURRENDERER_H_INCLUDED
#define GAUSSIANBLURRENDERER_H_INCLUDED
#include "OpenGLDrawer.h"

class GaussianBlurRenderer : public OpenGLDrawer
{
public:
    GaussianBlurRenderer(OpenGLContext& glContext,
                      int screenWidth,
                      int screenHeight,
                      int amount);
    void draw() override;
    void BindTexture(GLuint textureID)
    {
        outputTexture.set(0, textureID);
    }
private:
    virtual const char* getVertexShader() override;
    virtual const char* getFragmentShader() override;
    
    ScreenShape screenShape;
    const int Amount;
    ScopedPointer<OpenGLShaderProgram::Uniform> textureID;
};



#endif  // GAUSSIANBLURRENDERER_H_INCLUDED
