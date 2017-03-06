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
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 normal;\n"
    "layout (location = 2) in vec4 sourceColour;\n"
    "layout (location = 3) in vec2 textureCoordIn;\n"
    "\n"
    "uniform mat4 modelMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 projectionMatrix;\n"
    "uniform float modelScale;\n"
    "\n"
    "out vec4 destinationColour;\n"
    "out vec2 TexCoords;\n"
    "out vec4 scolor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec3 pos = position;\n"
    "   destinationColour = sourceColour;\n"
    "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(modelScale * pos, 1.0);\n"
    //"   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(modelScale * pos, 1.0);"
    "   TexCoords = textureCoordIn;\n"
    "}";
}
const char* ScreenRenderer::getFragmentShader()
{
    return
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 destinationColour;\n"
    "in vec2 TexCoords;\n"
    "in vec4 scolor;\n"
    "\n"
    "uniform sampler2D Renderer;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   FragColor = destinationColour;\n"
    "}";
}
Matrix ScreenRenderer::getViewMatrix()
{
//    float yaw;
//    float pch;
    float azi;
    float elv;
    float dis;
    azi = -camAzimuth;
    elv = -camElevation;
    dis = -camDistance;
//    float cy = std::cos(-yaw), cp = std::cos(pch);;
//    float sy = std::sin(-yaw), sp = std::sin(pch);;
    float ca = std::cos(-azi), ce = std::cos(elv);
    float sa = std::sin(-azi), se = std::sin(elv);
    // Order of Distance * Elevation * Azimuth transform
    Matrix t(ca, -sa*se, -ce*sa, 0,
             0, ce, -se, 0,
             sa, ca*se, ca*ce, 0,
             0, 0, dis, 1);
    //    Matrix t2(cy, -sy*sp, -cp*sy, 0,
    //              0, cp, -sp, 0,
    //              sy, cy*sp, cy*cp, 0,
    //              0, 0, 0, 1);
    
//    Matrix t2(cy, 0, -sy, 0,
//              -sy*sp, cp, -cy*sp, 0,
//              cp*sy, sp, cy*cp, 0,
//              0, 0, 0, 1);
    //return isInterative?t:(t2*t);
    return t;
}
Matrix ScreenRenderer::getInverseViewMatrix()
{
    float azi;
    float elv;
    float dis;
    azi = camAzimuth;
    elv = camElevation;
    dis = camDistance;
    float ca = std::cos(azi), ce = std::cos(elv);
    float sa = std::sin(azi), se = std::sin(elv);
    // Order of Azimuth * Elevation * Distance transform
    Matrix t(ca, 0, -sa, 0,
             -sa*se, ce, -ca*se, 0,
             ce*sa, se, ca*ce, 0,
             dis*ce*sa, dis*se, dis*ca*ce, 1);
    return t;
}
Matrix ScreenRenderer::getProjectionMatrix()
{
    float n = 1;
    float f = 2000;
    float w = n * std::tan(viewAngleHorizon/2);
    float h = n * std::tan(viewAngleVertical/2);
    return Matrix::fromFrustum(-w, w, h, -h, n, f);
    ////////////////////////    Same with      ////////////
    //    return Matrix3D (n/w, 0,      0,             0,//
    //                     0,   n/h,    0,             0,//
    //                     0,   0,     -(f+n)/(f-n),  -1,//
    //                     0,   0,     -(2*f*n)/(f-n), 0);/
    ///////////////////////////////////////////////////////
}
Matrix ScreenRenderer::getInverseProjectionMatrix()
{
    float n = 1;
    float f = 2000;
    float w = n * std::tan(viewAngleHorizon/2);
    float h = n * std::tan(viewAngleVertical/2);
    return Matrix(w/n,  0,      0,  0,
                  0,    h/n,    0,  0,
                  0,    0,      0,  0.5*(n-f)/(f*n),
                  0,    0,      -1, 0.5*(f+n)/(f*n));
}
Matrix ScreenRenderer::getInverseTransformMatrix()
{
    Matrix v = getInverseViewMatrix();
    Matrix p = getInverseProjectionMatrix();
    return p * v;
}
Matrix ScreenRenderer::getTransformMatrix()
{
    Matrix v = getViewMatrix();
    Matrix p = getProjectionMatrix();
    return v * p;
}
ScreenRenderer::ScreenRenderer(OpenGLContext& glContext,
                               int screenWidth,
                                     int screenHeight) :
SimRenderer(glContext, screenWidth, screenHeight),
screenShape(glContext),
gridShape(glContext,0.8,8),
floorShape(glContext)
{
    float verticalRatio = (float)screenWidth/screenHeight;
    Matrix t;
    Matrix u(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    initializeShader(getVertexShader(), getFragmentShader());
    initializeUniformVariables();
    camAzimuth = 0;
    camElevation = M_PI_4*1.5f;
    camDistance = 4;
    viewAngleVertical = 60;
    viewAngleHorizon = std::atan(std::tan(viewAngleVertical/2)*verticalRatio)*2;
    //textureID = createUniform("Renderer");
    t = getProjectionMatrix();
    shaderProgram->use();
    projectionMatrix->setMatrix4(t.mat, 1, false);
}

void ScreenRenderer::draw()
{
    Matrix t;
    Matrix u(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    camAzimuth += 0.01f;
    OpenGLHelpers::clear (Colour::greyLevel (0.1f));
    shaderProgram->use();
    const float desktopScale = (float) context.getRenderingScale();
    t = getViewMatrix();
    viewMatrix->setMatrix4(t.mat, 1, false);
    modelMatrix->setMatrix4(u.mat, 1, false);
    modelScale->set(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width*desktopScale, height*desktopScale);
//    textureID->set(0);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, inputTexture);
    gridShape.draw();
    floorShape.draw();
//    screenShape.draw();
}