/*
  ==============================================================================

    PanoramaVideoRenderer.cpp
    Created: 21 Aug 2016 3:07:40pm
    Author:  Ricky

  ==============================================================================
*/

#include "PanoramaVideoRenderer.h"

const char* PanoramaVideoRenderer::getVertexShader()
{
    return
    "#version 330\n"
    "const float PI = 3.1415926536;\n"
    "\n"
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
    "uniform int isVideoMode;\n"
    "\n"
    "out vec4 destinationColour;\n"
    "out vec4 normalvect;\n"
    "out vec2 UV;\n"
    "out vec2 labelUV;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec4 origin;\n"
    "   vec4 pos;\n"
    "   float diff = 0;\n"
    "   float v = textureCoordIn.x;\n"
    "   float u = textureCoordIn.y;\n"
    "   vec3 aedposition = vec3(PI-2*PI*v, PI*0.5-PI*u, 790);\n"
    "   destinationColour = sourceColour;\n"
    "   if(isUniformColour == 1)\n"
    "       destinationColour = bodyColour;\n"
    "   normalvect = vec4(normal, 1.0);\n"
    "   labelUV = textureCoordIn;\n"
    "   if(isVideoMode==1)\n"
    "   {\n"
    "       UV = (projectionMatrix * inverse(viewMatrix) * vec4(aedposition, 1.0)).xy;\n"
    "       gl_Position = projectionMatrix * vec4(aedposition.xy, 800, 1.0);\n"
    "   }\n"
    "   else\n"
    "   {\n"
    "       origin = viewMatrix * modelMatrix * vec4(0,0,0,1);\n"
    "       if(nodeType==1)\n"
    "       {\n"
    "           if(origin.x<-PI)\n"
    "               diff = 2*PI;\n"
    "           else if(origin.x>PI)\n"
    "               diff = -2*PI;\n"
    "           origin.x += diff;\n"
    "           pos = origin + vec4(modelScale * position, 0);\n"
    "       }\n"
    "       else\n"
    "       {\n"
    "           pos = viewMatrix * vec4(aedposition,1.0);\n"
    "       }\n"
    "       if(modelMode==3)\n"
    "       {\n"
    "           pos = origin + vec4(position * 0.12,0.0) + vec4(0,-0.1,-0.1,0);\n"
    "       }\n"
    "       gl_Position = projectionMatrix * pos;\n"
    "   }\n"
    "}\n";
}
const char* PanoramaVideoRenderer::getFragmentShader()
{
    return
    "#version 330\n"
    "layout (location = 0) out vec4 FragColor;\n"
    "layout (location = 1) out vec4 idValue;\n"
    "in vec4 destinationColour;\n"
    "in vec4 normalvect;\n"
    "in vec2 UV;\n"
    "in vec2 labelUV;\n"
    "\n"
    "uniform int isVideoMode;\n"
    "uniform vec4 idColour;"
    "uniform int lineWidth;\n"
    "uniform int stereoScopicMode;\n"
    "uniform float nodeIndex;\n"
    "uniform int modelMode;\n"
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
    "       uv.x = (uv.x/2 + 0.5);\n"
    "       uv.y = -uv.y;\n"
    "       uv.y = uv.y/2+0.5;\n"
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
    "       uv.x = labelUV.x;\n"
    "       uv.y = (nodeNum-1 - nodeIndex + labelUV.y)/nodeNum;\n"
    "       FragColor = vec4(destinationColour.rgb+0.5,1.0) * texture(labelTexture, uv);\n"
    "       if(FragColor.a<0.1)\n"
    "           discard;\n"
    "   }\n"
    "   idValue = idColour;\n"
    "}";
}

Matrix PanoramaVideoRenderer::getViewMatrix(bool isInteractive)
{
    float azi;
    float elv;
    float dis;
    azi = -(getListenerYaw());
    elv = 0;
    dis = 0;
    Matrix t(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             azi, elv, dis, 1);
    return t;
}
Matrix PanoramaVideoRenderer::getViewMatrix2()
{
    float azi;
    float elv;
    float dis;
    azi = -(getListenerYaw());
    elv = 0;
    dis = 0;
    if(azi>0)
        azi-=2*M_PI;
    else if(azi<0)
        azi+=2*M_PI;
    Matrix t(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             azi, elv, dis, 1);
    return t;
}
Matrix PanoramaVideoRenderer::getInverseViewMatrix(bool isInteractive)
{
    float azi;
    float elv;
    float dis;
    azi = getListenerYaw();
    elv = 0;
    dis = 0;
    Matrix t(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             azi, elv, dis, 1);
    return t;
}
Matrix PanoramaVideoRenderer::getProjectionMatrix()
{
    float n = 0.1;
    float f = 2000;
    return Matrix(2/viewAngleHorizon, 0, 0, 0,
                  0, 2/viewAngleVertical, 0, 0,
                  0, 0, 2/(f-n), 0,
                  0, 0, -(f+n)/(f-n), 1);

}
Matrix PanoramaVideoRenderer::getInverseProjectionMatrix()
{
    float n = 0.1;
    float f = 2000;
    return Matrix(viewAngleHorizon/2, 0, 0, 0,
                  0, viewAngleVertical/2, 0, 0,
                  0, 0, (f-n)/2, 0,
                  0, 0, -(f+n)/2, 1);
}
Matrix PanoramaVideoRenderer::getInverseTransformMatrix()
{
    Matrix v = getInverseViewMatrix();
    Matrix p = getInverseProjectionMatrix();
    return p * v;
}
Matrix PanoramaVideoRenderer::getTransformMatrix()
{
    Matrix v = getViewMatrix();
    Matrix p = getProjectionMatrix();
    return v * p;
}
PanoramaVideoRenderer::PanoramaVideoRenderer(OpenGLContext& glContext,
                                 int screenWidth,
                                 int screenHeight,
                                 DataModel& nodelist) :
PanoramicNodeRenderer(glContext, screenWidth, screenHeight, nodelist),
globeShape(glContext, 800),
gridShape(glContext, 790, 6)
{
    initializeShader(getVertexShader(), getFragmentShader());
    initializeUniformVariables();
    
    videoTexture = createUniform("video");
    isVideoMode = createUniform("isVideoMode");
    stereoScopicMode = createUniform("stereoScopicMode");
    shaderProgram->use();
    
    camAzimuth = 0;
    camElevation = 0;
    camDistance = 2000;
    viewAngleVertical = M_PI;
    viewAngleHorizon = 2 * M_PI;
    nodeSize = 0.2;
    Matrix t = getProjectionMatrix();
    projectionMatrix->setMatrix4(t.mat, 1, false);
}
void PanoramaVideoRenderer::paint()
{
    Matrix t;
    Matrix u(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    modelMatrix->setMatrix4(u.mat, 1, false);
    videoTexture->set(0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexture);
    
    if(NodeList.isShowVideo)
    {
        isVideoMode->set(1);
        drawShape(globeShape, NodeType::NONE, Colour(0xFF000000));
        isVideoMode->set(0);
    }
    if(NodeList.isTrackNameShow)
    {
        labelTexture->set(1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, labelTextureID);
        drawLabels(0.5, 3);
    }
    drawAmbisonicController(0.0125f);
    drawChannelNodes(PANORAMAVIEW_CHANNEL_NODE_SIZE);
    drawObjectNodes(0.5, 3);
    
    if(NodeList.isShowGrid)
    {
        drawShape(gridShape, NodeType::NONE, Colour(0xFF000000), Colour(0x11555555));
        viewMatrix->setMatrix4(getViewMatrix2().mat, 1, false);
        drawShape(gridShape, NodeType::NONE, Colour(0xFF000000), Colour(0x11555555));
    }
}
void PanoramaVideoRenderer::draw()
{
    Matrix t;
    Matrix u(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    float nnum;
    shaderProgram->use();
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    OpenGLHelpers::clear (Colour(BACKGROUND_MAIN));// Enable depth test
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    t = getProjectionMatrix();
    projectionMatrix->setMatrix4(t.mat, 1, false);
    t = getViewMatrix();
    for(nnum = 1.0f ; nnum<NodeList.getSize() ; nnum*=2.0f);
    nodeNum->set(nnum);
    viewMatrix->setMatrix4(t.mat, 1, false);
    stereoScopicMode->set(static_cast<int>(NodeList.StereoScopicMode));
    paint();
    selectNode();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}