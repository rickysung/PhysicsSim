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


class ScreenRenderer : public OpenGLDrawer
{
public:
    ScreenRenderer(OpenGLContext& glContext,
                      int screenWidth,
                      int screenHeight);
    void draw() override;
    void BindTexture(GLuint textureID)
    {
        inputTexture = textureID;
    }
private:
    GLuint inputTexture;
    virtual const char* getVertexShader() override;
    virtual const char* getFragmentShader() override;
    ScreenShape screenShape;
    ScopedPointer<OpenGLShaderProgram::Uniform> textureID;
};




#endif  // SCREENRENDERER_H_INCLUDED
