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

#include "AEDObject.h"
#include "YPRObject.h"
#include "ChannelNode.h"
class NodeRenderer : public OpenGLDrawer
{
public:
    NodeRenderer(OpenGLContext& glContext,
                  int screenWidth,
                  int screenHeight,
                  DataModel& nodelist) :
    OpenGLDrawer(glContext, screenWidth, screenHeight, 3),
    flatObjectShape(glContext),
    objectShape(glContext),
    channelShape(glContext),
    ambiShape(glContext),
    controllerShape(glContext),
    NodeList(nodelist)
    {
        genFrameBuffer();
    }
    void initializeUniformVariables()
    {
        modelMatrix = createUniform("modelMatrix");
        viewMatrix = createUniform("viewMatrix");
        projectionMatrix = createUniform("projectionMatrix");
        bodyColour = createUniform("bodyColour");
        idColour = createUniform("idColour");
        nodeType = createUniform("nodeType");
        isUniformColour = createUniform("isUniformColour");
        modelScale = createUniform("modelScale");
        lineMode = createUniform("lineMode");
        lineWidth = createUniform("lineWidth");
    }
    virtual Matrix getTransformMatrix() = 0;
    virtual Matrix getInverseTransformMatrix() = 0;
    virtual Matrix getViewMatrix() = 0;
    virtual Matrix getProjectionMatrix() = 0;
    virtual Matrix getInverseProjectionMatrix() = 0;
    virtual Matrix getInverseViewMatrix() = 0;
    
    void setMousePosition(int x, int y) { mx = x, my= y, isSelectMode=true; }
    virtual float getListenerYaw() { return camAzimuth; }
    virtual float getListenerPitch() { return camElevation; }
    virtual float getCameraDistance() { return camDistance; }
    virtual void setListenerYaw(float val) { camAzimuth = val; }
    virtual void setListenerPitch(float val) { camElevation = val; }
    virtual void setCameraDistance(float val) { camDistance = val; }
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
    ScopedPointer<OpenGLShaderProgram::Uniform> lineMode;
    
    virtual Matrix getModelMatrixAtPosition(PositionalObject* node, float ns, float azi, float elv, float dis) = 0;
    
    void selectNode()
    {
        if(isSelectMode)
        {
            GLint cx, cy;
            glBindTexture(GL_TEXTURE_2D, outputTexture[1]);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &cx);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &cy);
            size_t data_size = 4 * cx * cy;
            data = new GLubyte[data_size];
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            GLubyte ObjectType = data[(width*my+mx)*4];
            int nodeID = data[(width*my+mx)*4+2];
            int nodeData = data[(width*my+mx)*4+1];
            
            switch(ObjectType)
            {
                case (int)NodeType::OBJECT:
                {
                    NodeList.focusOn(nodeID);
                    break;
                }
                case (int)NodeType::AMBISONIC:
                {
                    YPRObject* ambiNode = dynamic_cast<YPRObject*>(NodeList.getFocusedNode());
                    int selecttype = nodeData >> 4;
                    ambiNode->setControlMode(static_cast<SelectType>(selecttype));
                    break;
                }
                default:
                {
                    break;
                }
            }
            isSelectMode = false;
            delete[] data;
        }
    }
    virtual void setModelMatrix(PositionalObject& node, float scale, float minDis, float maxDis) = 0;
    void drawShape(Shape& shapeToDraw,
                   NodeType nodetype,
                   Colour iColour,
                   Colour bColour = Colour(0x00000000))
    {
        nodeType->set(static_cast<int>(nodetype));
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
    void drawChannelNodes(float cubeSize)
    {
        int i;
        float ns;
        Colour id;
        Colour c;
        ChannelNode* cnode;
        PositionalObject* node;
        Matrix t;
        ChannelType cType;
        float mag = getMagnifyCoefficient();
        for(i=0 ; i<NodeList.getSize() ; i++)
        {
            if(NodeList[i]->isVisible() && NodeList[i]->getNodeType() == NodeType::CHANNEL)
            {
                ns = cubeSize;
                if(i==NodeList.getFocusedNodeIndex())
                    ns = cubeSize*mag;
                cnode = dynamic_cast<ChannelNode*>(NodeList[i]);
                node = dynamic_cast<PositionalObject*>(NodeList[i]);
                cType = cnode->getChannelType();
                c = NodeList[i]->getColour();
                
                switch (cType)
                {
                    case ChannelType::STEREO:
                    {
                        modelScale->set(ns/DEFAULT_CHANNEL_NODE_DISTANCE);
                        
                        t = getModelMatrixAtPosition(node, ns, -M_PI/6, 0, DEFAULT_CHANNEL_NODE_DISTANCE);
                        modelMatrix->setMatrix4(t.mat, 1, false);
                        drawShape(channelShape, NodeType::OBJECT, Colour(0xFF031000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
                        
                        t = getModelMatrixAtPosition(node, ns, M_PI/6, 0, DEFAULT_CHANNEL_NODE_DISTANCE);
                        modelMatrix->setMatrix4(t.mat, 1, false);
                        drawShape(channelShape, NodeType::OBJECT, Colour(0xFF031000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
                        break;
                    }
                    case ChannelType::CHANNEL_5_1:
                    {
                        modelScale->set(ns/DEFAULT_CHANNEL_NODE_DISTANCE);
                        
                        t = getModelMatrixAtPosition(node, ns, 0, 0, DEFAULT_CHANNEL_NODE_DISTANCE);
                        modelMatrix->setMatrix4(t.mat, 1, false);
                        drawShape(channelShape, NodeType::OBJECT, Colour(0xFF031000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
                        
                        t = getModelMatrixAtPosition(node, ns, M_PI/6, 0, DEFAULT_CHANNEL_NODE_DISTANCE);
                        modelMatrix->setMatrix4(t.mat, 1, false);
                        drawShape(channelShape, NodeType::OBJECT, Colour(0xFF031000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
                        
                        t = getModelMatrixAtPosition(node, ns, -M_PI/6, 0, DEFAULT_CHANNEL_NODE_DISTANCE);
                        modelMatrix->setMatrix4(t.mat, 1, false);
                        drawShape(channelShape, NodeType::OBJECT, Colour(0xFF031000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
                        
                        t = getModelMatrixAtPosition(node, ns, 110 * M_PI/180, 0, DEFAULT_CHANNEL_NODE_DISTANCE);
                        modelMatrix->setMatrix4(t.mat, 1, false);
                        drawShape(channelShape, NodeType::OBJECT, Colour(0xFF031000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
                        
                        t = getModelMatrixAtPosition(node, ns, -110 * M_PI/180, 0, DEFAULT_CHANNEL_NODE_DISTANCE);
                        modelMatrix->setMatrix4(t.mat, 1, false);
                        drawShape(channelShape, NodeType::OBJECT, Colour(0xFF031000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
                        
                        break;
                    }
                    case ChannelType::CHANNEL_7_1:
                        break;
                    default:
                        break;
                }
            }
        }
    }
    void drawController(SoundNode* fnode, float ns)
    {
        PositionalObject* node = dynamic_cast<PositionalObject*>(fnode);
        Colour c = fnode->getColour();
        int n = 72;
        float diff = 2*M_PI/n;
        Matrix t;
        int i;
        modelScale->set(ns/8);
        
        t = getModelMatrixAtPosition(node, ns*3, 0, 0, 10);
        modelMatrix->setMatrix4(t.mat, 1, false);
        drawShape(objectShape, NodeType::OBJECT, Colour(0xFF021000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
        
        t = getModelMatrixAtPosition(node, ns*3, M_PI, 0, 10);
        modelMatrix->setMatrix4(t.mat, 1, false);
        drawShape(objectShape, NodeType::OBJECT, Colour(0xFF021000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
        
        for(i=0 ; i<n ; i++)
        {
            t = getModelMatrixAtPosition(node, ns, 0, (i)*diff, 10);
            modelMatrix->setMatrix4(t.mat, 1, false);
            drawShape(objectShape, NodeType::OBJECT, Colour(0xFF021000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
        }
        for(i=0 ; i<n ; i++)
        {
            t = getModelMatrixAtPosition(node, ns, (i)*diff, 0, 10);
            modelMatrix->setMatrix4(t.mat, 1, false);
            drawShape(objectShape, NodeType::OBJECT, Colour(0xFF021000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), c);
        }
    }
    float getMagnifyCoefficient()
    {
        float mag;
        int c = NodeList.AnimationCounter%20;
        switch(static_cast<int>(c/10))
        {
            case 0:
            {
                mag = static_cast<float>(c)/10;
                break;
            }
            case 1:
            {
                mag = static_cast<float>(20-c)/10;
                break;
            }
        }
        return 1.01 + mag*0.6f;
    }
    void drawAmbisonicController()
    {
        int i;
        float ns = 0.1f;
        float mag = getMagnifyCoefficient();
        for(i=0 ; i<NodeList.getSize() ; i++)
        {
            if(NodeList[i]->isVisible() && NodeList[i]->getNodeType() == NodeType::AMBISONIC)
            {
                if(i== NodeList.getFocusedNodeIndex())
                    drawController(NodeList[i], ns* mag);
                else
                    drawController(NodeList[i], ns);
            }
        }
    }
    void drawAmbisonicNodes(bool isAEDMode = false)
    {
        /////////////////
        // Not Use Yet //
        /////////////////
        //        Colour id;
        //        PositionalObject* node = dynamic_cast<PositionalObject*>(NodeList.getFocusedNode());
        //        //        if(isAEDMode)
        //        //            setAEDModelMatrix(*node, 80, 0.1, 10);
        //        //        else
        //        setModelMatrix(*node, 80, 0.1, 10);
        //        drawShape(ambiShape, NodeType::AMBISONIC, Colour(0xFF020000 + static_cast<unsigned int>(NodeList.getFocusedNodeIndex())), Colour(0xFF222222));
    }
    virtual void drawObjectNodes(float minDis, float maxDis) = 0;
    
    FlatObjectShape flatObjectShape;
    ObjectShape objectShape;
    SpeakerShape channelShape;
    AmbisonicShape ambiShape;
    AmbisonicControllerShape controllerShape;
    GLubyte *data;
    int mx= 0, my = 0;
    bool isSelectMode = false;
    DataModel& NodeList;
};


#endif  // NODEVIEWRENDERER_H_INCLUDED
