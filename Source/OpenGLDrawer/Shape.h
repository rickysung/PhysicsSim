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
struct ObjShape : public Shape
{
public:
    ObjShape(OpenGLContext& openGLContext, const char* binary_data, const char* mat_binary_data) : Shape(openGLContext)
    {
        shapeFile.load(binary_data, mat_binary_data);
        Array<Colour> colourmap = {Colours::darkgrey, Colours::grey, Colours::white};
        for(int i=0 ; i<shapeFile.modelObjects.size() ; i++)
        {
            VertexArrays.add(new WavefrontVertex(shapeFile.modelObjects.getUnchecked(i)->mesh, colourmap[i]));
            VertexArrays.getLast()->initShape(context);
        }
    }
    ~ObjShape()
    {
        for(int i=0 ; i<shapeFile.modelObjects.size() ; i++)
        {
            VertexArrays.getUnchecked(i)->deleteShape(context);
        }
    }
    void draw(OpenGLShaderProgram::Uniform* matAmbi,
              OpenGLShaderProgram::Uniform* matDiff,
              OpenGLShaderProgram::Uniform* matSpec,
              OpenGLShaderProgram::Uniform* matShin)
    {
        WavefrontObjFile::Vertex amb;
        WavefrontObjFile::Vertex dif;
        WavefrontObjFile::Vertex spc;
        float shn;
        const char* s;
        for(int i=0 ; i<shapeFile.modelObjects.size() ; i++)
        {
            s = shapeFile.modelObjects.getUnchecked(i)->material.name.getCharPointer();
            
            amb = shapeFile.modelObjects.getUnchecked(i)->material.ambient;
            dif = shapeFile.modelObjects.getUnchecked(i)->material.diffuse;
            spc = shapeFile.modelObjects.getUnchecked(i)->material.specular;
            shn = shapeFile.modelObjects.getUnchecked(i)->material.shininess;
            
            matAmbi->set(amb.x, amb.y, amb.z);
            matDiff->set(dif.x, dif.y, dif.z);
            matSpec->set(spc.x, spc.y, spc.z);
            matShin->set(shn);
            context.extensions.glBindVertexArray(VertexArrays.getUnchecked(i)->shapeVAO);
            glDrawElements(GL_TRIANGLES, VertexArrays.getUnchecked(i)->vertexSize, GL_UNSIGNED_INT, 0);
            context.extensions.glBindVertexArray(0);
        }
    }
    void draw() override
    {
        for(int i=0 ; i<shapeFile.modelObjects.size() ; i++)
        {
            context.extensions.glBindVertexArray(VertexArrays.getUnchecked(i)->shapeVAO);
            glDrawElements(GL_TRIANGLES, VertexArrays.getUnchecked(i)->vertexSize, GL_UNSIGNED_INT, 0);
            context.extensions.glBindVertexArray(0);
        }
    }
    WavefrontObjFile shapeFile;
    OwnedArray<WavefrontVertex> VertexArrays;
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
struct HeadOrientShape : public Shape
{
public:
    HeadOrientShape(OpenGLContext& openGLContext, float length) : Shape(openGLContext)
    {
        HeadVector.initShape(context, 1.3f, Colour(Colours::white), Vector(0, 0, length));
        NoseVector.initShape(context, 1.3f, Colour(Colours::white), Vector(0, length, 0));
    }
    ~HeadOrientShape()
    {
        HeadVector.deleteShape(context);
        NoseVector.deleteShape(context);
    }
    void draw() override
    {
        context.extensions.glBindVertexArray(HeadVector.shapeVAO);
        glDrawElements(GL_TRIANGLES, HeadVector.vertexSize, GL_UNSIGNED_INT, 0);
        
        context.extensions.glBindVertexArray(NoseVector.shapeVAO);
        glDrawElements(GL_TRIANGLES, NoseVector.vertexSize, GL_UNSIGNED_INT, 0);
    }
private:
    LineVertex HeadVector;
    LineVertex NoseVector;
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
        X.initShape(context, 1, Colour(GRID_LINE), Vector(div * diff, 0, 0), Vector(diff, 0, 0));
        X2.initShape(context, 1, Colour(GRID_LINE), Vector(-div * diff, 0, 0), Vector(-diff, 0, 0));
        //Y.initShape(context, 1, Colour(GRID_LINE), Vector(0, div*diff, 0), Vector(0, -div*diff, 0));
        Z.initShape(context, 1, Colour(GRID_LINE), Vector(0, 0, div*diff), Vector(0, 0, diff));
        Z2.initShape(context, 1, Colour(GRID_LINE), Vector(0, 0, -div*diff), Vector(0, 0, -diff));
    }
    ~PolarGridShape()
    {
        int i;
        for(i=0 ; i<division ; i++)
        {
            gridEllipses[i].deleteShape(context);
        }
        X.deleteShape(context);
        X2.deleteShape(context);
        //Y.deleteShape(context);
        Z.deleteShape(context);
        Z2.deleteShape(context);
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
        
        context.extensions.glBindVertexArray(Z.shapeVAO);
        glDrawElements(GL_TRIANGLES, Z.vertexSize, GL_UNSIGNED_INT, 0);
        
        context.extensions.glBindVertexArray(X2.shapeVAO);
        glDrawElements(GL_TRIANGLES, X2.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(Z2.shapeVAO);
        
        glDrawElements(GL_TRIANGLES, Z2.vertexSize, GL_UNSIGNED_INT, 0);
        
        context.extensions.glBindVertexArray(0);
    }
private:
    const int division;
    EllipseVertex* gridEllipses;
    LineVertex X;
    LineVertex Z;
    LineVertex X2;
    LineVertex Z2;
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
struct RoadShape : public Shape
{
    RoadShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        roadVertex.initShape(context);
    }
    ~RoadShape()
    {
        roadVertex.deleteShape(context);
    }
    void draw() override
    {
        context.extensions.glBindVertexArray(roadVertex.shapeVAO);
        glDrawElements(GL_TRIANGLES, roadVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
    Array<Vector>& getRoadPoints()
    {
        return roadVertex.pointArray;
    }
private:
    RoadVertex roadVertex;
    
};
struct FloorShape : public Shape
{
    FloorShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        quadVertex.initShape(context);
    }
    ~FloorShape()
    {
        quadVertex.deleteShape(context);
    }
    void draw () override
    {
        context.extensions.glBindVertexArray(quadVertex.shapeVAO);
        glDrawElements(GL_LINES, quadVertex.vertexSize, GL_UNSIGNED_INT, 0);
        context.extensions.glBindVertexArray(0);
    }
private:
    FloorVertex quadVertex;
};
struct LabelShape : public Shape
{
    LabelShape(OpenGLContext& openGLContext) : Shape(openGLContext)
    {
        quadVertex.initShape(context, 3.5f, 0.4375f);
    }
    ~LabelShape()
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
struct ScreenShape : public Shape
{
public:
    ScreenShape(OpenGLContext& openGLContext, float x, float y, float w, float h) : Shape(openGLContext)
    {
        quadVertex.initShape(context, x, y, w, h);
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
