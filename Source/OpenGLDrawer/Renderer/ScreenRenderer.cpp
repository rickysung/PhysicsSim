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
    "layout (location = 4) in mat4 instanceMatrix;\n"
    "\n"
    "uniform mat4 modelMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 projectionMatrix;\n"
    "uniform float modelScale;\n"
    "uniform vec4 bodyColour;\n"
    "uniform int modelMode;\n"
    "\n"
    "out vec4 destinationColour;\n"
    "out vec2 TexCoords;\n"
    "out vec4 scolor;\n"
    "out vec3 FragNorm;\n"
    "out vec3 lightPos;\n"
    "out vec3 FragPos;\n"
    "out vec2 UV;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   vec3 lp = vec3(0, 2000, 200);\n"
    "   vec3 pos = position;\n"
    "   destinationColour = bodyColour;\n"
    "   lightPos = vec3(viewMatrix * vec4(lp, 0.0));\n"
    "   TexCoords = textureCoordIn;\n"
    "   switch(modelMode)\n"
    "   {\n"
    "       case 1:\n"
    "       {\n"
    "           gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(modelScale * pos, 1.0);\n"
    "           FragNorm = vec3(viewMatrix * modelMatrix * modelScale * vec4(normal, 0.0));\n"
    "           FragPos = vec3(viewMatrix * modelMatrix * vec4(modelScale * pos, 0.0));\n"
    "           break;\n"
    "       }\n"
    "       case 2:\n"
    "       {\n"
    "           gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(modelScale * pos + 0.1*normal, 1.0);\n"
    "           FragNorm = vec3(viewMatrix * modelMatrix * modelScale * vec4(normal, 0.0));\n"
    "           FragPos = vec3(viewMatrix * modelMatrix * vec4(modelScale * pos + 0.1*normal, 0.0));\n"
    "           break;\n"
    "       }\n"
    "   }\n"
    "}";
}
const char* ScreenRenderer::getFragmentShader()
{
    return
    "#version 330 core\n"
    "struct Material {\n"
    "    vec3 ambient;\n"
    "    vec3 diffuse;\n"
    "    vec3 specular;\n"
    "    float shininess;\n"
    "};\n"
    "\n"
    "struct Light {\n"
    "    vec3 position;\n"
    "\n"
    "    vec3 ambient;\n"
    "    vec3 diffuse;\n"
    "    vec3 specular;\n"
    "};\n"
    "\n"
    "out vec4 FragColor;\n"
    "in vec4 destinationColour;\n"
    "in vec2 TexCoords;\n"
    "in vec4 scolor;\n"
    "in vec3 FragNorm;\n"
    "in vec2 UV;\n"
    "in vec3 FragPos;\n"
    "in vec3 lightPos;\n"
    "\n"
    "uniform sampler2D Renderer;\n"
    "uniform int isMaterialMode;\n"
    "uniform Material material;\n"
    "uniform Light light;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   if(isMaterialMode>=1)\n"
    "   {\n"
    "       vec3 viewPos = vec3(0.0, 0.0, 1.0);\n"
    
    // Ambient
    "       vec3 ambient = light.ambient * material.ambient;\n"
    
    // Diffuse
    "       vec3 norm = normalize(FragNorm);\n"
    "       vec3 lightDir = normalize(lightPos - FragPos);\n"
    "       float diff = max(dot(norm, lightDir), -dot(norm, lightDir));\n"
    "       vec3 diffuse = light.diffuse * (diff * material.diffuse);\n"
    
    // Specular
    "       vec3 viewDir = normalize(viewPos - FragPos);\n"
    "       vec3 reflectDir = reflect(-lightDir, norm);\n"
    "       float spec = pow(max(dot(viewDir, reflectDir), -dot(viewDir, reflectDir)), material.shininess);\n"
    "       vec3 specular = light.specular * (spec * material.specular);\n"
    "\n"
    "       vec3 result = ambient + diffuse + specular;\n"
    "       FragColor = vec4(result, 1.0);\n"
    "       if(isMaterialMode==2)\n"
    "           FragColor = vec4(result, 0.5);\n"
    "   }\n"
    "   else\n"
    "   {"
    "       float zBuffer = gl_FragCoord.w;\n"
    "       FragColor = vec4(1.0, 1.0, 1.0, (zBuffer*3)<1.0?(zBuffer*3):1.0);\n"
    "       if(zBuffer<0.01)\n"
    "           discard;\n"
    "   }"
    "}";
}
Matrix ScreenRenderer::getViewMatrix()
{
    Matrix c = carBody.getCarState().getInverseLocationMatrix();
    
    float yaw;
    float pch;
    float azi;
    float elv;
    float dis;
    azi = -camAzimuth;
    elv = -camElevation;
    dis = -camDistance;
    yaw = -camYaw;
    pch = -camPitch;
    float cy = std::cos(yaw), cp = std::cos(pch);;
    float sy = std::sin(yaw), sp = std::sin(pch);;
    float ca = std::cos(azi), ce = std::cos(elv);
    float sa = std::sin(azi), se = std::sin(elv);
    Matrix t(ca, -sa*se, -ce*sa, 0,
                                                          0, ce, -se, 0,
                                                          sa, ca*se, ca*ce, 0,
                                                          0, 0, dis, 1);
    Matrix t2(1,0,0,0,
              0,cp,-sp,0,
              0,sp,cp,0,
              0,0,0,1);
    Matrix t3(cy,0,-sy,0,
              0,1,0,0,
              sy,0,cy,0,
              0,0,0,1);
    return c * t * t2 * t3;
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
    float n = 1.0f;
    float f = 2000.0f;
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
    float n = 1.0f;
    float f = 2000.0f;
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
                               CarBody& cb,
                               int screenWidth,
                                     int screenHeight) :
SimRenderer(glContext, screenWidth, screenHeight),
screenShape(glContext),
gridShape(glContext,0.8,8),
floorShape(glContext),
carShape(glContext, BinaryData::avent_obj, BinaryData::avent2_mtl),
wheelShape(glContext, BinaryData::aventWheel_obj, BinaryData::aventWheel_mtl),
roadShape(glContext),
carBody(cb)
{
    float verticalRatio = (float)screenWidth/screenHeight;
    Matrix t;
    Matrix u(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    initializeShader(getVertexShader(), getFragmentShader());
    initializeUniformVariables();
    camAzimuth = M_PI;
    camElevation = M_PI_2;
    camDistance = 10;
    camYaw = 0;
    camPitch = 0;
    viewAngleVertical = 30;
    viewAngleHorizon = std::atan(std::tan(viewAngleVertical/2)*verticalRatio)*2;
    //textureID = createUniform("Renderer");
    t = getProjectionMatrix();
    shaderProgram->use();
    projectionMatrix->setMatrix4(t.mat, 1, false);
    ambientLoc->set(1.0f, 0.5f, 0.32f);
    diffuseLoc->set(1.0f, 0.5f, 0.32f);
    specularLoc->set(0.5f, 0.5f, 0.5f);
    shineLoc->set(2.0f);
    lightAmbientLoc->set(0.2f, 0.2f, 0.2f);
    lightDiffuseLoc->set(0.9f, 0.9f, 0.9f);
    lightSpecularLoc->set(1.0f, 1.0f, 1.0f);
    
    max_save_state = 10;
    state_idx = 0;
}
Matrix ScreenRenderer::getWheelMatrix(CarState state, TIRE_INDEX idx)
{
    switch (idx) {
        case TIRE_INDEX::FRONT_LEFT:
            return state.getWheelMatrix(-carBody.getFrontWheelTrack(), carBody.getFrontWheelBase(), static_cast<int>(idx), true);
            break;
        case TIRE_INDEX::FRONT_RIGHT:
            return state.getWheelMatrix(carBody.getFrontWheelTrack(), carBody.getFrontWheelBase(), static_cast<int>(idx), true);
            break;
        case TIRE_INDEX::REAR_LEFT:
            return state.getWheelMatrix(-carBody.getRearWheelTrack(), -carBody.getRearWheelBase(), static_cast<int>(idx), false);
            break;
        case TIRE_INDEX::REAR_RIGHT:
            return state.getWheelMatrix(carBody.getRearWheelTrack(), -carBody.getRearWheelBase(), static_cast<int>(idx), false);
            break;
        default:
            break;
    }

}
void ScreenRenderer::drawCar(CarState state)
{
    Matrix trs = state.getLocationMatrix();
    Matrix rearRightWheel = getWheelMatrix(state, TIRE_INDEX::REAR_RIGHT);
    Matrix rearLeftWheel = getWheelMatrix(state, TIRE_INDEX::REAR_LEFT);
    Matrix frontRightWheel = getWheelMatrix(state, TIRE_INDEX::FRONT_RIGHT);
    Matrix frontLeftWheel = getWheelMatrix(state, TIRE_INDEX::FRONT_LEFT);
    
    modelScale->set(1.0f);
    modelMatrix->setMatrix4(trs.mat, 1, false);
    bodyColour->set(1.0f,0.0f,0.0f,1.0f);
    carShape.draw(ambientLoc, diffuseLoc, specularLoc, shineLoc);
    bodyColour->set(0.3f,0.3f,0.3f,1.0f);
    
    modelMatrix->setMatrix4((rearRightWheel*trs).mat, 1, false);
    wheelShape.draw(ambientLoc, diffuseLoc, specularLoc, shineLoc);
    
    modelMatrix->setMatrix4((rearLeftWheel*trs).mat, 1, false);
    wheelShape.draw(ambientLoc, diffuseLoc, specularLoc, shineLoc);
    
    modelMatrix->setMatrix4((frontRightWheel*trs).mat, 1, false);
    wheelShape.draw(ambientLoc, diffuseLoc, specularLoc, shineLoc);
    
    modelMatrix->setMatrix4((frontLeftWheel*trs).mat, 1, false);
    wheelShape.draw(ambientLoc, diffuseLoc, specularLoc, shineLoc);
}
void ScreenRenderer::saveState()
{
    CarState t(carBody.getCarState());
    if(carStates.size()==max_save_state)
    {
        carStates.set(state_idx,t);
        state_idx=(++state_idx)%max_save_state;
    }
    else
    {
        carStates.add(t);
    }
}
void ScreenRenderer::draw()
{
    int i, n;
    Matrix t;
    Matrix u(1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
    
    OpenGLHelpers::clear (Colours::skyblue);
    shaderProgram->use();
    const float desktopScale = (float) context.getRenderingScale();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glViewport(0, 0, width*desktopScale, height*desktopScale);
    t = getViewMatrix();
    viewMatrix->setMatrix4(t.mat, 1, false);
    modelMatrix->setMatrix4(u.mat, 1, false);
    modelScale->set(1.0f);
    bodyColour->set(0.0f,0.0f,1.0f,1.0f);
    isMaterialMode->set(0);
    floorShape.draw();
    modelMode->set(2);
    isMaterialMode->set(1);
    roadShape.draw();
    modelMode->set(1);
    drawCar(carBody.getCarState());
    n = carStates.size();
    isMaterialMode->set(2);
    for(i=0 ; i<n ; i++)
    {
        drawCar(carStates[i]);
    }
    isMaterialMode->set(0);
    
}