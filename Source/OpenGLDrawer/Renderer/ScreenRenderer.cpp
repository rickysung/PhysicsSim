/*
  ==============================================================================

    ScreenRenderer.cpp
    Created: 20 Aug 2016 8:19:03pm
    Author:  Ricky

  ==============================================================================
*/

#include "ScreenRenderer.h"
const char* ScreenRenderer::getVertexShader()
{
    return
    "#version 330 core\n"
    "layout (location = 0) in vec4 position;\n"
    "layout (location = 1) in vec4 normal;\n"
    "layout (location = 2) in vec4 sourceColour;\n"
    "layout (location = 3) in vec2 textureCoordIn;\n"
    "\n"
    "out vec2 TexCoords;\n"
    "out vec4 scolor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = position;\n"
    "    TexCoords = textureCoordIn;\n"
    "}";
}
const char* ScreenRenderer::getFragmentShader()
{
    return
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoords;\n"
    "in vec4 scolor;\n"
    "\n"
    "uniform sampler2D Renderer;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec3 result = texture(Renderer, TexCoords).rgb;\n"
    "   FragColor = vec4(result,1.0);\n"
    "}";
}
ScreenRenderer::ScreenRenderer(OpenGLContext& glContext,
                                     int screenWidth,
                                     int screenHeight) :
OpenGLDrawer(glContext, screenWidth, screenHeight, 1),
screenShape(glContext, 0.8, 0.8, 0.2, 0.2)
{
    initializeShader(getVertexShader(), getFragmentShader());
    textureID = createUniform("Renderer");
    genFrameBuffer();
}

void ScreenRenderer::draw()
{
    OpenGLHelpers::clear (Colour::greyLevel (0.1f));// Enable depth test
    shaderProgram->use();
    const float desktopScale = (float) context.getRenderingScale();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width*desktopScale, height*desktopScale);
    textureID->set(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexture);
    screenShape.draw();
}