/*
  ==============================================================================

    PanoramicNodeRenderer.h
    Created: 2 Jan 2017 9:31:26pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef PANORAMICNODERENDERER_H_INCLUDED
#define PANORAMICNODERENDERER_H_INCLUDED


#include "NodeRenderer.h"


class PanoramicNodeRenderer : public NodeRenderer
{
public:
    PanoramicNodeRenderer(OpenGLContext& glContext, int screenWidth, int screenHeight, DataModel& nodelist) : NodeRenderer(glContext, screenWidth, screenHeight, nodelist)
    {}
protected:
    void setModelMatrix(PositionalObject& node, float scale, float minDis, float maxDis) override
    {
        float x, y, z;
        float dis;
        float azi, elv;
        Matrix t = node.getModelMatrixFromOrigin(scale, minDis, maxDis);
        if(t.mat[14]==0)
        {
            t = Matrix(1,0,0,0,
                       0,1,0,0,
                       0,0,1,0,
                       0,0,maxDis,1) * t;
        }
        x = t.mat[12];
        y = t.mat[13];
        z = t.mat[14];
        Vector v;
        dis = std::sqrt(x*x+y*y+z*z);
        azi = std::atan2(-x,z);
        elv = std::asin(y/dis);
        v = Vector(azi, elv, dis);
        modelMatrix->setMatrix4(Matrix(v).mat, 1, false);
    }
    void drawObjectNodes(float minDis, float maxDis) override
    {
        int i;
        float d;
        float mag = getMagnifyCoefficient();
        PositionalObject* node;
        
        for(i=0 ; i<NodeList.getSize() ; i++)
        {
            if(NodeList[i]->isVisible() && NodeList[i]->getNodeType() == NodeType::OBJECT)
            {
                float ns = nodeSize;
                if(NodeList[i]->isFocused())
                    ns = nodeSize * mag;
                
                node = dynamic_cast<PositionalObject*>(NodeList[i]);
                d = dynamic_cast<AEDObject*>(NodeList[i])->getDistance();
                d = d>maxDis?maxDis:(d<minDis?minDis:d);
                modelScale->set(ns/d);
                setModelMatrix(*node, ns, minDis, maxDis);
                drawShape(flatObjectShape, NodeType::OBJECT, Colour(0xFF010000 + static_cast<unsigned int>(i)), NodeList[i]->getColour());
            }
        }
    }
    Matrix getModelMatrixAtPosition(Matrix baseMatrix, float ns, float azi, float elv, float dis) override final
    {
        float x, y, z;
        Matrix q;
        Matrix p;
        Matrix r;
        Matrix s;
        Matrix t;
        Vector v;
        float ca, sa, ce, se;
        ca = std::cos(azi);
        sa = std::sin(azi);
        ce = std::cos(elv);
        se = std::sin(elv);
//        r = Matrix(1, 0, 0, 0,
//                   0, 1, 0, 0,
//                   0, 0, 1, 0,
//                   0, 0, dis,1);
//        p = Matrix(1,0,0,0,
//                   0,ce,-se,0,
//                   0,se,ce,0,
//                   0,0,0,1);
//        q = Matrix(ca,0,-sa,0,
//                   0,1,0,0,
//                   sa,0,ca,0,
//                   0,0,0,1);
        r = Matrix(ca, 0, -sa, 0,
                   -se*sa, ce, -ca*se,0,
                   ce*sa, se, ce*ca, 0,
                   dis*ce*sa, dis*se, dis*ce*ca, 1);
        t = r*baseMatrix;
        x = t.mat[12];
        y = t.mat[13];
        z = t.mat[14];
        v = Vector(std::atan2(-x, z), std::asin(y/dis), dis);
        return Matrix(v);
    }
};


#endif  // PANORAMICNODERENDERER_H_INCLUDED
