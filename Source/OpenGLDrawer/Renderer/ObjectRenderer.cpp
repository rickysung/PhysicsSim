//
//  VideoGlobeRenderer.cpp
//  GaudiReaper
//
//  Created by Ricky on 2016. 8. 13..
//
//
#include "ObjectRenderer.h"

const char* ObjectRenderer::getVertexShader()
{
    return
    "#version 330\n"
    "const float PI = 3.1415926535897932384626433832795;\n"
    "\n"
    "vec3 getAED(vec3 pos)\n"
    "{\n"
    "   float dis = length(pos);\n"
    "   float azi = atan(pos.x/pos.z);\n"
    "   float elv = asin(pos.y/dis);\n"
    "   if(pos.z<0)\n"
    "       azi+=PI;\n"
    "   if(azi<0)\n"
    "       azi = 2*PI + azi;\n"
    "   return vec3(azi, elv, dis);\n"
    "}\n"
    "vec3 getXYZ(vec3 aed)\n"
    "{\n"
    "   float x = aed[2] * cos(aed[1]) * sin(aed[0]);\n"
    "   float y = aed[2] * sin(aed[1]);\n"
    "   float z = aed[2] * cos(aed[1]) * cos(aed[0]);\n"
    "   return vec3(x, y, z);\n"
    "}\n"
    "vec4 getFOAfunc(vec3 aed)\n"
    "{\n"
    "   float ca = cos(aed[0]);\n"
    "   float ce = cos(aed[1]);\n"
    "   float sa = sin(aed[0]);\n"
    "   float se = sin(aed[1]);\n"
    "   float w = 1;\n"
    "   float x = ce*sa;\n"
    "   float y = se;\n"
    "   float z = ce*ca;\n"
    "   return vec4(w, x, y, z);\n"
    "}\n"
    "\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 normal;\n"
    "layout (location = 2) in vec4 sourceColour;\n"
    "layout (location = 3) in highp vec2 textureCoordIn;\n"
    "\n"
    "uniform mat4 modelMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 projectionMatrix;\n"
    "uniform int nodeType;\n"
    "uniform int isUniformColour;\n"
    "uniform vec4 bodyColour;\n"
    "uniform float modelScale;\n"
    "\n"
    "uniform vec3 gain[16];\n"
    "uniform int gainNum;\n"
    "uniform int modelMode;\n"
    "uniform int lineWidth;\n"
    "\n"
    "out vec4 destinationColour;\n"
    "out vec3 FragNorm;\n"
    "out vec2 UV;\n"
    "out vec3 FragPos;\n"
    "out vec3 lightPos;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec3 lp = vec3(0, 200, 1000);\n"
    "   vec3 org;\n"
    "   vec3 pos;\n"
    "   int i;\n"
    "   destinationColour = sourceColour;\n"
    "   UV = vec2(textureCoordIn.x, (textureCoordIn.y));\n"
    "   FragNorm = normal;\n"
    "   if(isUniformColour == 1)\n"
    "   {\n"
    "       destinationColour = bodyColour;\n"
    "   }\n"
    "   switch(nodeType)\n"
    "   {\n"
//    "       case 2:\n"
//    "       {\n"
//    "           vec4 B;\n"
//    "           vec3 aed0 = getAED(position);\n"
//    "           vec3 aed1 = vec3(aed0.x-0.1, aed0.y, aed0.z);\n"
//    "           vec3 aed2 = vec3(aed0.x, aed0.y-0.1, aed0.z);\n"
//    "           vec3 ov = position;\n"
//    "           vec3 av = getXYZ(aed1);\n"
//    "           vec3 bv = getXYZ(aed2);\n"
//    "           float gain0 = 0;\n"
//    "           float gain1 = 0;\n"
//    "           float gain2 = 0;\n"
//    "           if(gainNum>0)\n"
//    "           {\n"
//    "               B = getFOAfunc(gain[0]);\n"
//    "               gain0 = 0.5 * dot(B, getFOAfunc(aed0));\n"
//    "               gain1 = 0.5 * dot(B, getFOAfunc(aed1));\n"
//    "               gain2 = 0.5 * dot(B, getFOAfunc(aed2));\n"
//    "           }\n"
//    "           for(i=1 ; i<gainNum ; i++)\n"
//    "           {\n"
//    "               B = getFOAfunc(gain[i]);\n"
//    "               gain0 *= 0.5 * dot(B, getFOAfunc(aed0));\n"
//    "               gain1 *= 0.5 * dot(B, getFOAfunc(aed1));\n"
//    "               gain2 *= 0.5 * dot(B, getFOAfunc(aed2));\n"
//    "           }\n"
//    "           vec3 r1 = (1 - gain0) * position;\n"
//    "           vec3 r2 = (1 - gain1) * av;\n"
//    "           vec3 r3 = (1 - gain2) * bv;\n"
//    "           vec3 n = normalize(cross(r2-r1, r3-r1));\n"
//    "           FragNorm = vec3(viewMatrix * modelMatrix * vec4(n, 0.0));\n"
//    "           pos = r1;\n"
//    "           destinationColour = vec4(gain0, gain0, gain0, gain0);\n"
//    "           break;\n"
//    "       }\n"
    "       case 1:\n"
    "       case 3:\n"
    "       default:\n"
    "       {\n"
    "           pos = position;\n"
    "           FragNorm = vec3(viewMatrix * modelMatrix * modelScale * vec4(normal, 0.0));\n"
    "           break;\n"
    "       }\n"
    "   }\n"
    "   FragPos = vec3(viewMatrix * modelMatrix * vec4(modelScale * pos, 1.0));\n"
    "   lightPos = lp;\n"
    "   switch(modelMode)\n"
    "   {\n"
    "       case 2:\n"
    "       {\n"
    "           pos += 0.07*normal;\n"
    "           gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);\n"
    "           break;\n"
    "       }\n"
    "       case 3:\n"
    "       {\n"
    "           FragPos = vec3(0,0,0);\n"
    "           gl_Position = projectionMatrix * (viewMatrix * modelMatrix * vec4(0, 0, 0, 1.0) + vec4(pos,0.0) + vec4(0,-1,0,0));\n"
    "           break;\n"
    "       }\n"
    "       case 1:\n"
    "       default:\n"
    "       {\n"
    "           gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(modelScale * pos, 1.0);"
    "           break;\n"
    "       }\n"
    "   }\n"
    "}\n";
}
const char* ObjectRenderer::getFragmentShader()
{
    return
    "#version 330\n"
    "layout (location = 0) out vec4 FragColor;\n"
    "layout (location = 1) out vec4 idValue;\n"
    "layout (location = 2) out vec4 BrightColor;\n"
    "\n"
    "\n"
    "in vec4 destinationColour;\n"
    "in vec3 FragNorm;\n"
    "in vec2 UV;\n"
    "in vec3 FragPos;\n"
    "in vec3 lightPos;\n"
    "\n"
    "uniform vec4 idColour;\n"
    "uniform int isLight;\n"
    "uniform int modelMode;\n"
    "uniform float nodeIndex;\n"
    "uniform float nodeNum;\n"
    "\n"
    "uniform sampler2D labelTexture;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   float unused = nodeIndex;\n"
    "   float unused2 = nodeNum;\n"
    "   vec2 uv;\n"
    "   uv.x = UV.x;\n"
    "   uv.y = (nodeNum-1-nodeIndex + UV.y)/nodeNum;\n"
    "   if(isLight==1)\n"
    "   {\n"
    "       vec3 norm = normalize(FragNorm);\n"
    "       vec3 lightDir = normalize(lightPos-FragPos);\n"
    "       float diff = 0.7 * max(dot(norm, lightDir), 0.0);\n"
    "       vec3 res = (0.6 + diff) * destinationColour.rgb;\n"
    "       FragColor = vec4(res,1.0);\n"
    "   }\n"
    "   else\n"
    "   {\n"
    "       FragColor = destinationColour;\n"
    "   }\n"
    "   if(modelMode==3)\n"
    "   {\n"
    "       FragColor = vec4(destinationColour.rgb+0.5,1.0) * texture(labelTexture, uv);\n"
    "       if(FragColor.a<0.1)\n"
    "           discard;\n"
    "   }\n"
    "   idValue = idColour;\n"
    "   BrightColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}";
}
Matrix ObjectRenderer::getViewMatrix(bool isInterative)
{
    float yaw;
    float pch;
    float azi;
    float elv;
    float dis;
    azi = -camAzimuth;
    elv = -camElevation;
    dis = -camDistance;
    yaw = (NodeList.getListenerYaw());
    pch = (NodeList.getListenerPitch());
    float cy = std::cos(yaw), cp = std::cos(pch);;
    float sy = std::sin(yaw), sp = std::sin(pch);;
    float ca = std::cos(azi), ce = std::cos(elv);
    float sa = std::sin(azi), se = std::sin(elv);
    // Order of Distance * Elevation * Azimuth transform
    Matrix t(ca, -sa*se, -ce*sa, 0,
             0, ce, -se, 0,
             sa, ca*se, ca*ce, 0,
             0, 0, dis, 1);
//    Matrix t2(cy, -sy*sp, -cp*sy, 0,
//              0, cp, -sp, 0,
//              sy, cy*sp, cy*cp, 0,
//              0, 0, 0, 1);
    
    Matrix t2(cy, 0, -sy, 0,
             -sy*sp, cp, -cy*sp, 0,
             cp*sy, sp, cy*cp, 0,
             0, 0, 0, 1);
    return isInterative?t:(t2*t);
}
Matrix ObjectRenderer::getInverseViewMatrix(bool isInterative)
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
Matrix ObjectRenderer::getProjectionMatrix()
{
    float n = 1;
    float f = 2000;
    float w = n * std::tan(viewAngleHorizon/2);
    float h = n * std::tan(viewAngleVertical/2);
    return Matrix::fromFrustum(-w, w, -h, h, n, f);
////////////////////////    Same with      ////////////
//    return Matrix3D (n/w, 0,      0,             0,//
//                     0,   n/h,    0,             0,//
//                     0,   0,     -(f+n)/(f-n),  -1,//
//                     0,   0,     -(2*f*n)/(f-n), 0);/
///////////////////////////////////////////////////////
}
Matrix ObjectRenderer::getInverseProjectionMatrix()
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
Matrix ObjectRenderer::getInverseTransformMatrix()
{
    Matrix v = getInverseViewMatrix();
    Matrix p = getInverseProjectionMatrix();
    return p * v;
}
Matrix ObjectRenderer::getTransformMatrix()
{
    Matrix v = getViewMatrix();
    Matrix p = getProjectionMatrix();
    return v * p;
}
ObjectRenderer::ObjectRenderer(OpenGLContext& glContext,
                                     int screenWidth,
                                     int screenHeight,
                                     DataModel& nodelist,
                                     GworksMasterAudioProcessor& p) :
SpatialNodeRenderer(glContext, screenWidth, screenHeight, nodelist),
processor(p),
gridShape(glContext, 2, 5),
cursorShape(glContext),
headShape(glContext),
headOrientShape(glContext, 5)
{
    initializeShader(getVertexShader(), getFragmentShader());
    initializeUniformVariables();
    
    isLight = createUniform("isLight");
    gainNum = createUniform("gainNum");
    gains = createUniform("gain");
    shaderProgram->use();
    
    nodeSize = 0.4f;
    camAzimuth = M_PI;
    camElevation = M_PI_4;
    camDistance = 18;
    
    viewAngleHorizon = 60 * M_PI/180;
    viewAngleVertical = 60 * M_PI/180;
    
    Matrix t = getProjectionMatrix();
    projectionMatrix->setMatrix4(t.mat, 1, false);
    nodeType->set(0);
//    labelImages = new OpenGLTexture();
    
}

void ObjectRenderer::draw()
{
    OwnedArray<SoundNode> focusedNodes = NodeList.getObjectFocusedNode();
    const float desktopScale = (float) context.getRenderingScale();
    Matrix t;
    Matrix d(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    Matrix u(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    Colour id;
    Colour nodeColour;
    float nnum;
    shaderProgram->use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLHelpers::clear(Colour(BACKGROUND_MAIN));// Enable depth test
    glViewport(0, 0, width*desktopScale, height*desktopScale);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    t = getViewMatrix(false);
    
    for(nnum = 1.0f ; nnum<NodeList.getSize() ; nnum*=2.0f);
    nodeNum->set(nnum);
    
    viewMatrix->setMatrix4(t.mat, 1, false);
    modelMatrix->setMatrix4(d.mat, 1, false);
    modelScale->set(1.0f);
    isLight->set(1);
    drawShape(headShape, NodeType::NONE, Colour(0xFF000000));
    
    modelMode->set(static_cast<int>(ModelMode::LINE));
    drawShape(headOrientShape, NodeType::NONE, Colour(0xFF000000));
    t = getViewMatrix(true);
    viewMatrix->setMatrix4(t.mat, 1, false);
    drawShape(gridShape, NodeType::NONE, Colour(0xFF000000));
    modelMode->set(static_cast<int>(ModelMode::COMMON));
    
    Matrix cp;
    modelMatrix->setMatrix4(u.mat, 1, false);
    for(int i = focusedNodes.size() ; i-->0 ;)
    {
        if(focusedNodes.getUnchecked(i)->isVisible())
        {
            AEDObject* node = dynamic_cast<AEDObject*>(focusedNodes.getUnchecked(i));
            cp = node->getModelMatrixFromOrigin();
            cursorShape.setCursorPoint(Vector(cp.mat[12]/cp.mat[15], cp.mat[13]/cp.mat[15], cp.mat[14]/cp.mat[15]));
            drawShape(cursorShape, NodeType::NONE, Colour(0xFF000000), Colour(0xFFFFFF00));
        }
    }
    if(NodeList.isTrackNameShow)
    {
        labelTexture->set(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, labelTextureID);
        drawLabels();
    }
    drawChannelNodes(MAPVIEW_CHANNEL_NODE_SIZE);
    drawAmbisonicController(0.1f);
    drawObjectNodes();
    
    isLight->set(0);
//    selectNode();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    focusedNodes.clear(false);
}
