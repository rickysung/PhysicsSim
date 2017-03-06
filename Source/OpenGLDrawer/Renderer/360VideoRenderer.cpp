/*
  ==============================================================================

    360VideoRenderer.cpp
    Created: 18 Aug 2016 3:33:32pm
    Author:  Ricky

  ==============================================================================
*/

#include "360VideoRenderer.h"

const char* GlobeVideoRenderer::getVertexShader()
{
    return
    "#version 330\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 normal;\n"
    "layout (location = 2) in vec4 sourceColour;\n"
    "layout (location = 3) in vec2 textureCoordIn;\n"
    "\n"
    "uniform mat4 modelMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 projectionMatrix;\n"
    "uniform int nodeType;\n"
    "uniform int isUniformColour;\n"
    "uniform vec4 bodyColour;\n"
    "uniform int modelMode;\n"
    "uniform float modelScale;\n"
    "\n"
    "out vec4 destinationColour;\n"
    "out vec4 normalvect;\n"
    "out vec2 UV;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   float unused = nodeType;\n"
    "   vec3 pos;\n"
    "   UV = textureCoordIn;\n"
    "   destinationColour = sourceColour;\n"
    "   if(isUniformColour == 1)\n"
    "       destinationColour = bodyColour;\n"
    "   normalvect = vec4(normal, 1.0);\n"
    "   pos = position;\n"
    "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(modelScale * pos, 1.0);\n"
    "   if(modelMode == 3)\n"
    "   {\n"
    "       pos.x /=2;\n"// screen is 2 : 1
    "       gl_Position = projectionMatrix * (viewMatrix * modelMatrix * vec4(0, 0, 0, 1.0)) + vec4(pos*0.04,0.0) + vec4(0,-0.05,-0.1,0);\n"
    "   }\n"
    "}\n";
}
const char* GlobeVideoRenderer::getFragmentShader()
{
    return
    "#version 330\n"
    "layout (location = 0) out vec4 FragColor;\n"
    "layout (location = 1) out vec4 idValue;\n"
    "in vec4 destinationColour;\n"
    "in vec4 normalvect;\n"
    "in vec2 UV;\n"
    "\n"
    "uniform vec4 idColour;"
    "uniform int isVideoMode;\n"
    "uniform int stereoScopicMode;\n"
    "uniform int modelMode;\n"
    "uniform float nodeIndex;\n"
    "uniform float nodeNum;\n"
    "\n"
    "uniform sampler2D video;\n"
    "uniform sampler2D labelTexture;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec2 uv = UV;\n"
    "   if(isVideoMode==1)\n"
    "   {\n"
    "       switch(stereoScopicMode)\n"
    "       {\n"
    "           case 1:\n"
    "           {\n   "
    "               uv.x/=2;\n"
    "               if(uv.x>0.5)\n"
    "                   uv.x-=0.5;\n"
    "               break;\n"
    "           }\n"
    "           case 2:\n"
    "           {\n   "
    "               uv.x/=2;\n"
    "               uv.x+=0.5;\n"
    "               if(uv.x>1)\n"
    "                   uv.x-=0.5;\n"
    "               break;\n"
    "           }\n"
    "           case 3:\n"
    "           {\n   "
    "               uv.y/=2;\n"
    "               break;\n"
    "           }\n"
    "           case 4:\n"
    "           {\n   "
    "               uv.y/=2;\n"
    "               uv.y+=0.5;\n"
    "               break;\n"
    "           }\n"
    "           default:\n"
    "               break;\n"
    "       }\n"
    "       FragColor = vec4(texture(video, uv).rgb, 1.0);\n"
    "   }\n"
    "   else\n"
    "       FragColor = destinationColour;\n"
    "   if(modelMode==3)\n"
    "   {\n"
    "       uv.x = UV.x;\n"
    "       uv.y = (nodeNum-1-nodeIndex + UV.y)/nodeNum;\n"
    "       FragColor = vec4(destinationColour.rgb+0.5,1.0) * texture(labelTexture, uv);\n"
    "       if(FragColor.a<0.1)\n"
    "           discard;\n"
    "   }\n"
    "   idValue = idColour;\n"
    "}";
}
Matrix GlobeVideoRenderer::getViewMatrix(bool isInterative)
{
    float yaw;
    float pch;
    float azi;
    float elv;
    float dis;
    azi = -camAzimuth;
    elv = -camElevation;
    dis = -camDistance;
    yaw = -(NodeList.getListenerYaw());
    pch = -(NodeList.getListenerPitch());
    //printf("azi : %.2lf elv : %.2lf\n",NodeList.getListenerYaw() * 180*M_1_PI, NodeList.getListenerPitch() *180*M_1_PI);
    float cy = std::cos(-yaw), cp = std::cos(pch);;
    float sy = std::sin(-yaw), sp = std::sin(pch);;
    float ca = std::cos(-azi), ce = std::cos(elv);
    float sa = std::sin(-azi), se = std::sin(elv);
    // Order of Distance * Elevation * Azimuth transform
    Matrix t(ca, -sa*se, -ce*sa, 0,
             0, ce, -se, 0,
             sa, ca*se, ca*ce, 0,
             0, 0, dis, 1);
    Matrix t2(cy, -sy*sp, -cp*sy, 0,
             0, cp, -sp, 0,
             sy, cy*sp, cy*cp, 0,
             0, 0, 0, 1);
    return t2*t;
}
Matrix GlobeVideoRenderer::getInverseViewMatrix(bool isInterative)
{
    float azi;
    float elv;
    float dis;
    azi = NodeList.getListenerYaw();
    elv = (NodeList.getListenerPitch());
    dis = camDistance;
    float ca = std::cos(-azi), ce = std::cos(elv);
    float sa = std::sin(-azi), se = std::sin(elv);
    // Order of Azimuth * Elevation * Distance transform
    Matrix t(ca, 0, -sa, 0,
             -sa*se, ce, -ca*se, 0,
             ce*sa, se, ca*ce, 0,
             dis*ce*sa, dis*se, dis*ca*ce, 1);
    return t;
}
Matrix GlobeVideoRenderer::getProjectionMatrix()
{
    float n = 0.09;
    float f = globeRadius;
    float w = n * std::tan(viewAngleHorizon/2);
    float h = n * std::tan(viewAngleVertical/2);
    return Matrix::fromFrustum(-w, w, -h, h, n, f);
}
Matrix GlobeVideoRenderer::getInverseProjectionMatrix()
{
    float n = 0.09;
    float f = globeRadius;
    float w = n * std::tan(viewAngleHorizon/2);
    float h = n * std::tan(viewAngleVertical/2);
    return Matrix(w/n, 0, 0, 0,
                  0,h/n, 0, 0,
                  0, 0, 0, 0.5*(n-f)/(f*n),
                  0, 0, -1, 0.5*(f+n)/(f*n));
}
Matrix GlobeVideoRenderer::getInverseTransformMatrix()
{
    Matrix v = getInverseViewMatrix();
    Matrix p = getInverseProjectionMatrix();
    return p * v;
}
Matrix GlobeVideoRenderer::getTransformMatrix()
{
    Matrix v = getViewMatrix();
    Matrix p = getProjectionMatrix();
    return v * p;
}
GlobeVideoRenderer::GlobeVideoRenderer(OpenGLContext& glContext,
                                 int screenWidth,
                                 int screenHeight,
                                 DataModel& nodelist) :
SpatialNodeRenderer(glContext, screenWidth, screenHeight, nodelist),
globeShape(glContext, 800),
gridShape(glContext, 790, 18)
{
    initializeShader(getVertexShader(), getFragmentShader());
    initializeUniformVariables();
    
    videoTexture = createUniform("video");
    stereoScopicMode = createUniform("stereoScopicMode");
    isVideoMode = createUniform("isVideoMode");

    shaderProgram->use();
    
    camAzimuth = M_PI;
    camElevation = 0;
    camDistance = 0;
    globeRadius = 800;
    viewAngleVertical = 60 * M_PI/180;
    viewAngleHorizon = std::atan(std::tan(viewAngleVertical/2)*2)*2;
    nodeSize = 0.04;
    Matrix t = getProjectionMatrix();
    projectionMatrix->setMatrix4(t.mat, 1, false);
}
void GlobeVideoRenderer::draw()
{
    Matrix t;
    Matrix u(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    shaderProgram->use();
    float nnum;
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    OpenGLHelpers::clear (Colour(BACKGROUND_MAIN));// Enable depth test
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    t = getViewMatrix();
    viewMatrix->setMatrix4(t.mat, 1, false);
    modelMatrix->setMatrix4(u.mat, 1, false);
    modelScale->set(1.0f);
    for(nnum = 1.0f ; nnum<NodeList.getSize() ; nnum*=2.0f);
    nodeNum->set(nnum);
    videoTexture->set(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexture);
    if(NodeList.isShowVideo)
    {
        isVideoMode->set(1);
        stereoScopicMode->set(static_cast<int>(NodeList.StereoScopicMode));
        drawShape(globeShape, NodeType::NONE, Colour(0xFF000000));
        isVideoMode->set(0);
    }
    viewMatrix->setMatrix4(t.mat, 1, false);
    modelMatrix->setMatrix4(u.mat, 1, false);
    if(NodeList.isShowGrid)
        drawShape(gridShape, NodeType::NONE, Colour(0xFF000000), Colour(0x11555555));
    
    if(NodeList.isTrackNameShow)
    {
        labelTexture->set(1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, labelTextureID);
        drawLabels(0.5, 0.5);
    }
    drawAmbisonicController(0.1f);
    drawChannelNodes(HMDVIEW_CHANNEL_NODE_SIZE);
    drawObjectNodes(0.5f, 3);
    
    //selectNode();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}