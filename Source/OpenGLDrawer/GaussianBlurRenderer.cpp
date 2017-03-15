//
//  GaussianBlurRenderer.cpp
//  GaudiReaper
//
//  Created by Ricky on 2016. 8. 13..
//
//

#include "GaussianBlurRenderer.h"
const char* GaussianBlurRenderer::getVertexShader()
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
const char* GaussianBlurRenderer::getFragmentShader()
{
    return
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoords;\n"
    "in vec4 scolor;\n"
    "\n"
    "uniform sampler2D scene;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec3 result = texture(scene, TexCoords).rgb;\n"
    "   FragColor = vec4(result,1.0);\n"
    "}";
}
GaussianBlurRenderer::GaussianBlurRenderer(OpenGLContext& glContext,
                                     int screenWidth,
                                     int screenHeight,
                                     int amount) :
OpenGLDrawer(glContext, screenWidth, screenHeight, 1),
screenShape(glContext, 0, 0, 0.5, 0.5),
Amount(amount)
{
    initializeShader(getVertexShader(), getFragmentShader());
    textureID = createUniform("scene");
    genFrameBuffer();
}

void GaussianBlurRenderer::draw()
{
    OpenGLHelpers::clear (Colour::greyLevel (0.1f));// Enable depth test
    shaderProgram->use();
    
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    
    glViewport(0, 0, width, height);
    glUniform1i(textureID->uniformID, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, outputTexture[0]);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}