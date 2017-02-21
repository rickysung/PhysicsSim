/*
  ==============================================================================

    Shape.h
    Created: 28 Aug 2016 4:24:40pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

#include "ShapeVertex.h"

struct Shape
{
public:
    Shape(OpenGLContext& openGLContext) : context(openGLContext)
    {
    }
    virtual ~Shape(){}
    virtual void draw () = 0;
protected:
    OpenGLContext& context;
};
struct CursorShape : public Shape
{
public:
    CursorShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        cursorPointVertex.initShape(context);
    }
    ~CursorShape()
    {
        cursorPointVertex.deleteShape(context);
    }
    void setCursorPoint(Vector point)
    {
        cursorPointVertex.setPoint(context, point);
    }
    void draw () override
    {
        context.extensions.glBindVertexArray(cursorPointVertex.shapeVAO);
        glDrawElements(GL_LINE_LOOP, cursorPointVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    CursorVertex cursorPointVertex;
};


struct ObjectShape : public Shape
{
public:
    ObjectShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        objectVertex.initShape(context, 1.0);
    }
    ~ObjectShape()
    {
        objectVertex.deleteShape(context);
    }
    void draw () override
    {
        context.extensions.glBindVertexArray(objectVertex.shapeVAO);
        glDrawElements(GL_TRIANGLES, objectVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    SphereVertex objectVertex;
};
struct FlatObjectShape : public Shape
{
public:
    FlatObjectShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        objectVertex.initShape(context, 1.0);
    }
    ~FlatObjectShape()
    {
        objectVertex.deleteShape(context);
    }
    void draw() override
    {
        context.extensions.glBindVertexArray(objectVertex.shapeVAO);
        glDrawElements(GL_TRIANGLES, objectVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    FlatEllipseVertex objectVertex;
};
struct GlobeShape : public Shape
{
public:
    GlobeShape(OpenGLContext& openGLContext, float radius) : Shape(openGLContext)
    {
        globeVertex.initShape(context, radius);
    }
    ~GlobeShape()
    {
        globeVertex.deleteShape(context);
    }
    void draw () override
    {
        context.extensions.glBindVertexArray(globeVertex.shapeVAO);
        glDrawElements(GL_TRIANGLES, globeVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    SphereVertex globeVertex;
};

struct CartesianGridShape : public Shape
{
public:
    CartesianGridShape(OpenGLContext& openGLContext, int hDiv, int vDiv) : Shape(openGLContext), horizonDivision(hDiv), verticalDivision(vDiv)
    {
    }
    ~CartesianGridShape()
    {
        int i;
        for(i=0 ; i<lineNum ; i++)
        {
            gridLines[i].deleteShape(context);
        }
    }
    void draw () override
    {
//        int i;
  /*      for(i=0 ; i<division ; i++)
        {
            context.extensions.glBindVertexArray(gridEllipses[i].shapeVAO);
            glDrawElements(GL_LINE_LOOP, gridEllipses[i].vertexSize, GL_UNSIGNED_INT, 0);
            context.extensions.glBindVertexArray(0);
        }*/
    }
private:
    int lineNum=0;
    const int horizonDivision;
    const int verticalDivision;
    LineVertex* gridLines;
    
};
struct SphereGridShape : public Shape
{
    SphereGridShape(OpenGLContext& openGLContext, float radius, float div) : Shape(openGLContext)
    {
        gridVertex.initShape(context, radius, div);
    }
    ~SphereGridShape()
    {
        gridVertex.deleteShape(context);
    }
    void draw () override
    {
        context.extensions.glBindVertexArray(gridVertex.shapeVAO);
        glDrawElements(GL_LINES, gridVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    SphereGridVertex gridVertex;
};
struct PolarGridShape : public Shape
{
public:
    PolarGridShape(OpenGLContext& openGLContext, float diff, int div) : Shape(openGLContext), division(div)
    {
        int i;
        gridEllipses = new EllipseVertex[division];
        for(i=0 ; i<division ; i++)
        {
            gridEllipses[i].initShape(context, diff*(i+1));
        }
        X.initShape(context, 1, Colour(GRID_LINE), Vector(div * diff, 0, 0), Vector(-div * diff, 0, 0));
        Y.initShape(context, 1, Colour(GRID_LINE), Vector(0, div*diff, 0), Vector(0, -div*diff, 0));
        Z.initShape(context, 1, Colour(GRID_LINE), Vector(0, 0, div*diff), Vector(0, 0, -div*diff));
        HeadVector.initShape(context, 1.3f, Colour(Colours::white), Vector(0, 0, div * diff * 0.5));
        NoseVector.initShape(context, 1.3f, Colour(Colours::white), Vector(0, div * diff * 0.5, 0));
    }
    ~PolarGridShape()
    {
        int i;
        for(i=0 ; i<division ; i++)
        {
            gridEllipses[i].deleteShape(context);
        }
        X.deleteShape(context);
        Y.deleteShape(context);
        Z.deleteShape(context);
        HeadVector.deleteShape(context);
        NoseVector.deleteShape(context);
        delete[] gridEllipses;
    }
    void draw () override
    {
        int i;
        for(i=0 ; i<division ; i++)
        {
            context.extensions.glBindVertexArray(gridEllipses[i].shapeVAO);
            glDrawElements(GL_TRIANGLES, gridEllipses[i].vertexSize, GL_UNSIGNED_INT, 0);
        }
        context.extensions.glBindVertexArray(X.shapeVAO);
        glDrawElements(GL_TRIANGLES, X.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(Y.shapeVAO);
        glDrawElements(GL_TRIANGLES, Y.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(Z.shapeVAO);
        glDrawElements(GL_TRIANGLES, Z.vertexSize, GL_UNSIGNED_INT, 0);
        
        context.extensions.glBindVertexArray(HeadVector.shapeVAO);
        glDrawElements(GL_TRIANGLES, HeadVector.vertexSize, GL_UNSIGNED_INT, 0);
        
        context.extensions.glBindVertexArray(NoseVector.shapeVAO);
        glDrawElements(GL_TRIANGLES, NoseVector.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    const int division;
    EllipseVertex* gridEllipses;
    LineVertex X;
    LineVertex Y;
    LineVertex Z;
    LineVertex HeadVector;
    LineVertex NoseVector;
};
struct AmbisonicControllerShape : public Shape
{
    AmbisonicControllerShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        bandVertex.initShape(context);
    }
    ~AmbisonicControllerShape()
    {
        bandVertex.deleteShape(context);
    }
    void draw() override
    {
        context.extensions.glBindVertexArray(bandVertex.shapeVAO);
        glDrawElements(GL_TRIANGLES, bandVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    EllipseBandVertex bandVertex;
};
struct AmbisonicShape : public Shape
{
public:
    AmbisonicShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        sphereVertex.initShape(context);
    }
    ~AmbisonicShape()
    {
        sphereVertex.deleteShape(context);
    }
    void draw() override
    {
        context.extensions.glBindVertexArray(sphereVertex.shapeVAO);
        glDrawElements(GL_LINES, sphereVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    AmbisonicSphereVertex sphereVertex;
};

struct SpeakerShape : public Shape
{
public:
    SpeakerShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        cubeVertex.initShape(context);
    }
    ~SpeakerShape()
    {
        cubeVertex.deleteShape(context);
    }
    void draw() override
    {
        context.extensions.glBindVertexArray(cubeVertex.shapeVAO);
        glDrawElements(GL_TRIANGLES, cubeVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    CubeVertex cubeVertex;
};

struct FFMPEGTextureShape : public Shape
{
public:
    FFMPEGTextureShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        textureVertex.initShape(context);
    }
    ~FFMPEGTextureShape()
    {
        textureVertex.deleteShape(context);
    }
    void draw () override
    {
        context.extensions.glBindVertexArray(textureVertex.shapeVAO);
        glDrawElements(GL_TRIANGLES, textureVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    TextureVertex textureVertex;
};

struct ScreenShape : public Shape
{
public:
    ScreenShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        quadVertex.initShape(context);
    }
    ~ScreenShape()
    {
        quadVertex.deleteShape(context);
    }
    void draw () override
    {
        context.extensions.glBindVertexArray(quadVertex.shapeVAO);
        glDrawElements(GL_TRIANGLES, quadVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    RectangleVertex quadVertex;
};


#endif  // SHAPE_H_INCLUDED
