/*
  ==============================================================================

    SpatialNodeRenderer.h
    Created: 2 Jan 2017 9:31:10pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef SPATIALNODEDRAWER_H_INCLUDED
#define SPATIALNODEDRAWER_H_INCLUDED


#include "NodeRenderer.h"

class SpatialNodeRenderer : public NodeRenderer
{
public:
    SpatialNodeRenderer(OpenGLContext& glContext, int screenWidth, int screenHeight, DataModel& nodelist) : NodeRenderer(glContext, screenWidth, screenHeight, nodelist)
    {}
protected:
    void setModelMatrix(PositionalObject& node, float scale, float minDis, float maxDis) override
    {
        modelScale->set(scale);
        Matrix t = node.getModelMatrixFromOrigin(1, minDis, maxDis);
        if(t.mat[14]==0)
        {
            t = Matrix(1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,maxDis,1) * t;
        }
        modelMatrix->setMatrix4(t.mat, 1, false);
    }
    
    void drawObjectNodes(float minDis = 0.1, float maxDis = 10) override
    {
        int i;
        ObjectNode* onode;
        PositionalObject* node;
        ValueTree& objectNodes = NodeList.getObjectTree();
        float mag = getMagnifyCoefficient();
        
        for(i = objectNodes.getNumChildren() ; i-->0 ;)
        {
            onode = dynamic_cast<ObjectNode*>(objectNodes.getChild(i)["data"].getObject());
            if(onode->isVisible())
            {
                float ns = nodeSize;
                if(onode->isFocused())
                    ns = nodeSize * mag;
                
                node = dynamic_cast<PositionalObject*>(onode);
                setModelMatrix(*node, ns, minDis, maxDis);
                drawShape(objectShape, NodeType::OBJECT, Colour(0xFF010000 + static_cast<unsigned int>(i)), onode->getColour());
            }
        }
    }
    Matrix getModelMatrixAtPosition(Matrix baseMatrix, float ns, float azi, float elv, float dis) override
    {
        Matrix y;
        Matrix p;
        Matrix d;
        Matrix s;
        //Matrix body = node->getModelMatrixFromOrigin(1, 0.1, 10);
        float ca, sa, ce, se;
        ca = std::cos(azi);
        sa = std::sin(azi);
        ce = std::cos(elv);
        se = std::sin(elv);
//        s = Matrix(ns, 0, 0, 0,
//                   0, ns, 0, 0,
//                   0, 0, ns, 0,
//                   0, 0, 0, 1);
//        d = Matrix(1, 0, 0, 0,
//                   0, 1, 0, 0,
//                   0, 0, 1, 0,
//                   0, 0, dis,1);
//        p = Matrix(1,0,0,0,
//                   0,ce,-se,0,
//                   0,se,ce,0,
//                   0,0,0,1);
//        y = Matrix(ca,0,-sa,0,
//                   0,1,0,0,
//                   sa,0,ca,0,
//                   0,0,0,1);
        s = Matrix(ns*ca, 0, -ns*sa, 0,
                   -ns*se*sa, ns*ce, -ns*ca*se, 0,
                   ns*ce*sa, ns*se, ns*ce*ca, 0,
                   dis*ce*sa, dis*se, dis*ce*ca, 1);
        return s*baseMatrix;
    }
};



#endif  // SPATIALNODERENDERER_H_INCLUDED
