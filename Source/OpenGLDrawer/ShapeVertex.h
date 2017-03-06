/*
  ==============================================================================

    ShapeVertex.h
    Created: 28 Aug 2016 4:25:16pm
    Author:  Ricky

  ==============================================================================
*/

#ifndef SHAPEVERTEX_H_INCLUDED
#define SHAPEVERTEX_H_INCLUDED
#include "WavefrontObjParser.h"
struct Vertex
{
    float position[3];
    float normal[3];
    float colour[4];
    float texCoord[2];
};
struct ShapeVertex
{
    ShapeVertex(){}
    virtual ~ShapeVertex()
    {
    }
    void buildArcLine(Vector v1, Vector v2, float thickness)// p1 and p2 contains Azimuth Elevation Distance sequence
    {
        int i;
        float x1, y1, z1;
        float x2, y2, z2;
        Colour g = Colour(GRID_LINE);
        Vector p1, p2;
        Vector norm[8];
        Vertex vert;
        float a, e, d;
        int cnt = vertices.size() - 1;
        a = v1.x;
        e = v1.y;
        d = v1.z;
        x1 = d * std::cos(e) * std::sin(a);
        y1 = d * std::sin(e);
        z1 = d * std::cos(e) * std::cos(a);
        p1 = Vector(x1, y1, z1);
        a = v2.x;
        e = v2.y;
        d = v2.z;
        x2 = d * std::cos(e) * std::sin(a);
        y2 = d * std::sin(e);
        z2 = d * std::cos(e) * std::cos(a);
        p2 = Vector(x2, y2, z2);
        norm[0] = -p1/p1.length();
        norm[1] = (p1-p2)^p1;
        norm[1] /= norm[1].length();
        norm[2] = -norm[0];
        norm[3] = -norm[1];
        norm[4] = -p2/p2.length();
        norm[5] = p2^(p2-p1);
        norm[5] /= norm[5].length();
        norm[6] = -norm[4];
        norm[7] = -norm[5];
        
        for(i=0 ; i<4 ; i++)
        {
            norm[i]*=thickness;
            vert = {
                {x1, y1, z1},
                {norm[i].x, norm[i].y, norm[i].z},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5, 0.5}
            };
            vertices.add(vert);
        }
        for(i=4 ; i<8 ; i++)
        {
            norm[i]*=thickness;
            vert = {
                {x2, y2, z2},
                {norm[i].x, norm[i].y, norm[i].z},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5, 0.5}
            };
            vertices.add(vert);
        }
        indecies.add(cnt+1);
        indecies.add(cnt+2);
        indecies.add(cnt+5);
        
        indecies.add(cnt+2);
        indecies.add(cnt+5);
        indecies.add(cnt+6);
        
        indecies.add(cnt+2);
        indecies.add(cnt+3);
        indecies.add(cnt+6);
        
        indecies.add(cnt+3);
        indecies.add(cnt+6);
        indecies.add(cnt+7);
        
        indecies.add(cnt+3);
        indecies.add(cnt+4);
        indecies.add(cnt+7);
        
        indecies.add(cnt+4);
        indecies.add(cnt+7);
        indecies.add(cnt+8);
        
        indecies.add(cnt+1);
        indecies.add(cnt+4);
        indecies.add(cnt+5);
        
        indecies.add(cnt+4);
        indecies.add(cnt+5);
        indecies.add(cnt+8);
    }
    void buildStraightLine(Vector p1, Vector p2, float thickness, Colour g)
    {
        int i;
        Vector norm[8];
        Vertex vert;
        int cnt = vertices.size() - 1;
        norm[1] = (p1-p2)^(p1-Vector(-999,372,51));
        norm[1] /= norm[1].length();
        norm[0] = (p1-p2)^norm[1];
        norm[0] /= norm[0].length();
        norm[2] = -norm[0];
        norm[3] = -norm[1];
        norm[5] = (p2-Vector(-999,372,51))^(p2-p1);
        norm[5] /= norm[5].length();
        norm[4] = norm[5]^(p2-p1);
        norm[4] /= norm[4].length();
        norm[6] = -norm[4];
        norm[7] = -norm[5];
        for(i=0 ; i<4 ; i++)
        {
            norm[i]*=thickness;
            vert = {
                {p1.x, p1.y, p1.z},
                {norm[i].x, norm[i].y, norm[i].z},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5, 0.5}
            };
            vertices.add(vert);
        }
        for(i=4 ; i<8 ; i++)
        {
            norm[i]*=thickness;
            vert = {
                {p2.x, p2.y, p2.z},
                {norm[i].x, norm[i].y, norm[i].z},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5, 0.5}
            };
            vertices.add(vert);
        }
        indecies.add(cnt+1);
        indecies.add(cnt+2);
        indecies.add(cnt+5);
        
        indecies.add(cnt+2);
        indecies.add(cnt+5);
        indecies.add(cnt+6);
        
        indecies.add(cnt+2);
        indecies.add(cnt+3);
        indecies.add(cnt+6);
        
        indecies.add(cnt+3);
        indecies.add(cnt+6);
        indecies.add(cnt+7);
        
        indecies.add(cnt+3);
        indecies.add(cnt+4);
        indecies.add(cnt+7);
        
        indecies.add(cnt+4);
        indecies.add(cnt+7);
        indecies.add(cnt+8);
        
        indecies.add(cnt+1);
        indecies.add(cnt+4);
        indecies.add(cnt+5);
        
        indecies.add(cnt+4);
        indecies.add(cnt+5);
        indecies.add(cnt+8);
    }
    virtual void initShape(OpenGLContext& context, float cof1, float cof2, float cof3) = 0;
    void deleteShape(OpenGLContext& context)
    {
        context.extensions.glDeleteVertexArrays(1, &shapeVAO);
        context.extensions.glDeleteBuffers(1, &shapeVBO);
        context.extensions.glDeleteBuffers(1, &shapeEBO);
    }
    GLuint shapeVAO = 0;
    int vertexSize;
protected:
    Array<Vertex> vertices;
    Array<uint32> indecies;
    GLuint shapeVBO, shapeEBO;
    Vector getAED(Vector pos)
    {
        float dis = pos.length();
        if(pos.z == 0)
            pos.z = 1;
        if(dis == 0)
            dis = 1;
        float azi = std::atan(pos.x/pos.z);
        float elv = std::asin(pos.y/dis);
       if(pos.z<0)
       {
           if(pos.x<0)
           {
               azi-=M_PI;
           }
           else
           {
               azi+=M_PI;
           }
       }
       return Vector(azi, elv, dis);
    }
    void putData(OpenGLContext& context, Array<Vertex>& vertices, Array<uint32>& indecies)
    {
        context.extensions.glBindBuffer (GL_ARRAY_BUFFER, shapeVBO);
        context.extensions.glBufferData (GL_ARRAY_BUFFER,
                                         static_cast<GLsizeiptr> (static_cast<size_t> (vertices.size()) * sizeof (Vertex)),
                                         vertices.getRawDataPointer(), GL_STATIC_DRAW);
        
        // These will call OpenGL routines to create a buffer and store the indices in it
        context.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, shapeEBO);
        context.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER,
                                         static_cast<GLsizeiptr> (static_cast<size_t> (indecies.size()) * sizeof (juce::uint32)),
                                         indecies.getRawDataPointer(), GL_STATIC_DRAW);
    }
    void enableVertexArray(OpenGLContext& context)
    {
        context.extensions.glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), 0);
        context.extensions.glEnableVertexAttribArray (0);

        context.extensions.glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 3));
        context.extensions.glEnableVertexAttribArray (1);
        
        context.extensions.glVertexAttribPointer (2, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 6));
        context.extensions.glEnableVertexAttribArray (2);
        
        context.extensions.glVertexAttribPointer (3, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 10));
        context.extensions.glEnableVertexAttribArray (3);
    }
};

struct WavefrontVertex : ShapeVertex
{
    WavefrontVertex(WavefrontObjFile::Mesh& m, Colour c) : mesh(m), colour(c)
    {
    }
    void createVertexListFromMesh (const WavefrontObjFile::Mesh& mesh)
    {
        const float scale = 0.05f;
        WavefrontObjFile::TextureCoord defaultTexCoord = { 0.5f, 0.5f };
        WavefrontObjFile::Vertex defaultNormal = { 0.5f, 0.5f, 0.5f };
        //Colour colour = Colours::lightsalmon;
        for (int i = 0; i < mesh.vertices.size(); ++i)
        {
            int triangle_index = i/3;
            int idx;
            Vector one;
            Vector two;
            Vector three;
            Vector norm;
            idx = triangle_index * 3 + i%3;
            const WavefrontObjFile::Vertex& v1 = mesh.vertices.getReference (idx);
            idx = triangle_index * 3 + (i+1)%3;
            const WavefrontObjFile::Vertex& v2 = mesh.vertices.getReference (idx>=mesh.vertices.size()?(idx-1):idx);
            idx = triangle_index * 3 + (i+2)%3;
            const WavefrontObjFile::Vertex& v3 = mesh.vertices.getReference (idx>=mesh.vertices.size()?(idx-2):idx);
            one = Vector(v1.x, v1.y, v1.z);
            two = Vector(v2.x, v2.y, v2.z);
            three = Vector(v3.x, v3.y, v3.z);
            norm = (three-one) ^ (two-one);
            if((norm+one).length()<one.length())
                norm = -norm;
            defaultNormal = {norm.x, norm.y, norm.z};
            const WavefrontObjFile::Vertex& n
            = i < mesh.normals.size() ? mesh.normals.getReference (i) : defaultNormal;
            
            const WavefrontObjFile::TextureCoord& tc
            = i < mesh.textureCoords.size() ? mesh.textureCoords.getReference (i) : defaultTexCoord;
            
            Vertex vert =
            {
                { scale * v1.x, scale * v1.y, scale * v1.z, },
                { scale * n.x, scale * n.y, scale * n.z, },
                { colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() },
                { tc.x, tc.y }
            };
            vertices.add (vert);
        }
    }
    void initShape(OpenGLContext& context, float cof1 = 0, float cof2 = 0, float cof3 = 0) override
    {
        //WavefrontObjFile::Mesh& mesh = shapeFile.modelObjects.getUnchecked(2)->mesh;
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        createVertexListFromMesh(mesh);
        putData(context, vertices, mesh.indices);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = mesh.indices.size();
    }
    Colour colour;
    WavefrontObjFile::Mesh& mesh;
};
struct CubeVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 0, float cof2 = 0, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(GRID_LINE);
        int cnt = 0;
        vert = {{1, 1, -1},{0, 0, -1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, -1, -1},{0, 0, -1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, -1, -1},{0, 0, -1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        vert = {{-1, -1, -1},{0, 0, -1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, 1, -1},{0, 0, -1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, 1, -1},{0, 0, -1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{1, 1, 1},{0, 0, 1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, -1, 1},{0, 0, 1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, -1, 1},{0, 0, 1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{-1, -1, 1},{0, 0, 1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, 1, 1},{0, 0, 1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, 1, 1},{0, 0, 1},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{1, -1, 1},{0, -1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, -1, -1},{0, -1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, -1, -1},{0, -1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{-1, -1, -1},{0, -1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, -1, 1},{0, -1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, -1, 1},{0, -1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{1, 1, 1},{0, 1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, 1, -1},{0, 1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, 1, -1},{0, 1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{-1, 1, -1},{0, 1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, 1, 1},{0, 1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, 1, 1},{0, 1, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{1, 1, 1},{1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, 1, -1},{1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, -1, -1},{1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{1, -1, -1},{1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, -1, 1},{1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{1, 1, 1},{1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{-1, 1, 1},{-1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, 1, -1},{-1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, -1, -1},{-1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        vert = {{-1, -1, -1},{-1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, -1, 1},{-1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        vert = {{-1, 1, 1},{-1, 0, 0},{g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},{0.5, 0.5}};
        vertices.add(vert);
        indecies.add(cnt++);
        indecies.add(cnt++);
        indecies.add(cnt++);
        
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context,vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct CursorVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 0, float cof2 = 0, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(GRID_LINE);
        vert = {
            {0, 0, 0},
            {0, 0, 1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.5, 0.5}
        };
        vertices.add(vert);
        indecies.add(0);
        vert = {
            {200, 0, 200},
            {0, 0, 1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.5, 0.5}
        };
        vertices.add(vert);
        indecies.add(1);
        vert = {
            {200, 200, 200},
            {0, 0, 1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.5, 0.5}
        };
        vertices.add(vert);
        indecies.add(2);
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context,vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
    void setPoint(OpenGLContext& context, Vector p)
    {
        Vertex ground;
        Vertex pos;
        Colour g = Colour(GRID_LINE);
        Vector aed = getAED(p);
        ground = {
            {p.x, 0, p.z},
            {0, 0, 1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.5, 0.5}
        };
        pos = {
            {p.x, p.y, p.z},
            {0, 0, 1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.5, 0.5}
        };
        context.extensions.glBindVertexArray(shapeVAO);
        context.extensions.glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
        context.extensions.glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex), sizeof(Vertex), &ground);
        context.extensions.glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex)*2, sizeof(Vertex), &pos);
    }
};
struct LineVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 1, float cof2 = 0, float cof3 = 0) override
    {
        
    }
    void initShape(OpenGLContext& context, float thickness, Colour lineColour, Vector p1, Vector p2 = Vector(0,0,0))
    {
        buildStraightLine(p1, p2, thickness, lineColour);
//        Vector aed = getAED(Vector(cof1, cof2, cof3));
//        vert = {
//            {cof1, cof2, cof3},
//            {0, 0, 1},
//            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
//            {0.5, 0.5},
//            {aed.x,aed.y,aed.z}
//        };
//        vertices.add(vert);
//        vert = {
//            {-cof1, -cof2, -cof3},
//            {0, 0, 1},
//            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
//            {0.5, 0.5},
//            {aed.x,aed.y,aed.z}
//        };
//        vertices.add(vert);
//        indecies.add(0);
//        indecies.add(1);
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context,vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct SphereGridVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 1, float cof2 = 6, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(GRID_LINE);
        int i, j;
        const int n = cof2;
        float idx[100][50] = {0,};
        float diff = M_PI/n;
        float azi, elv;
        int cnt=0;
        float u;
        float v;
        for(i = 0 ; i<=2*n ; i++)
        {
            for(j=0 ; j<=n ; j++)
            {
                elv = M_PI_2 - diff * (float)j;
                azi = M_PI - diff * (float)i;
                
                u = static_cast<float>(j)/n;
                v = static_cast<float>(i)/(2*n);
                vert = {
                    {cof1*std::cos(elv) * std::sin(azi), cof1*std::sin(elv), cof1*std::cos(elv)*std::cos(azi)},
                    {std::cos(elv) * std::sin(azi), std::cos(elv), std::cos(elv)*std::cos(azi)},
                    {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                    {v, u}
                };
                vertices.add(vert);
                idx[i][j] = cnt++;
            }
        }
        for(j=0 ; j<n ; j++)
        {
            for(i=0 ; i<2*n ; i++)
            {
                indecies.add(idx[i][j]);
                indecies.add(idx[i][j+1]);
                indecies.add(idx[i][j]);
                indecies.add(idx[i+1][j]);
            }
        }
        for(j=0 ; j<n ; j++)
        {
            indecies.add(idx[i][j]);
            indecies.add(idx[i][j+1]);
        }
        for(i=0 ; i<2*n ; i++)
        {
            indecies.add(idx[i][j]);
            indecies.add(idx[i+1][j]);
        }
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context,vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct SphereVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 1, float cof2 = 0, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(GRID_LINE);
        int i, j;
        const int n = 15;
        float idx[n*2+1][n*2] = {0,};
        float diff = M_PI/n;
        float azi, elv;
        int cnt=0;
        float u;
        float v;
        for(i = 0 ; i<=2*n ; i++)
        {
            for(j=0 ; j<=n ; j++)
            {
                elv = M_PI_2 - diff * (float)j;
                azi = M_PI - diff * (float)i;
                
                u = static_cast<float>(j)/n;
                v = static_cast<float>(i)/(2*n);
                vert = {
                    {cof1*std::cos(elv) * std::sin(azi), cof1*std::sin(elv), cof1*std::cos(elv)*std::cos(azi)},
                    {std::cos(elv) * std::sin(azi), std::sin(elv), std::cos(elv)*std::cos(azi)},
                    {1.0, 1.0, 1.0, 1.0},
                    {v, u}
                };
                vertices.add(vert);
                idx[i][j] = cnt++;
            }
        }
        for(j=0 ; j<n ; j++)
        {
            for(i=0 ; i<2*n ; i++)
            {
                indecies.add(idx[i][j]);
                indecies.add(idx[i][j+1]);
                indecies.add(idx[i+1][j]);
                indecies.add(idx[i][j+1]);
                indecies.add(idx[i+1][j+1]);
                indecies.add(idx[i+1][j]);
            }
        }
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context,vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct AmbisonicSphereVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 1, float cof2 = 0, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(GRID_LINE);
        int i, j;
        const int n = 15;
        int idx[n*2+1][n*2] = {0,};
        float diff = M_PI/n;
        float azi, elv;
        int cnt=0;
        float u;
        float v;
        Vector val;
        float ca, ce;
        float sa, se;
        for(i = 0 ; i<=2*n ; i++)
        {
            for(j=0 ; j<=n ; j++)
            {
                elv = M_PI_2 - diff * (float)j;
                azi = M_PI - diff * (float)i;
                ca = std::cos(azi);
                ce = std::cos(elv);
                sa = std::sin(azi);
                se = std::sin(elv);
                u = static_cast<float>(j)/n;
                v = static_cast<float>(i)/(2*n);
                vert = {
                    {ce*sa, se, ce*ca},
                    {ce*sa, se, ce*ca},
                    {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                    {v, u}
                };
                vertices.add(vert);
                idx[i][j] = cnt++;
            }
        }
        for(i=0 ; i<2*n ; i++)
        {
            for(j=0 ; j<n ; j++)
            {
                int v1, v2, v3;
                v1 = idx[i][j];
                v2 = idx[i+1][j];
                v3 = idx[i][j+1];
//                int v4;
//                v4 = idx[i+1][j+1];
                indecies.add(v1);
                indecies.add(v2);
                indecies.add(v1);
                indecies.add(v3);
            }
        }
        for(i=0 ; i<=2*n ; i++)
        {
            Vertex& t = vertices.getReference(idx[i][n]);
            Vertex& v = vertices.getReference(idx[i][0]);
            t.normal[0] = v.normal[0];
            t.normal[1] = v.normal[1];
            t.normal[2] = v.normal[2];
        }
        for(j=0 ; j<=n ; j++)
        {
            Vertex& t = vertices.getReference(idx[2*n][j]);
            Vertex& v = vertices.getReference(idx[0][j]);
            t.normal[0] = v.normal[0];
            t.normal[1] = v.normal[1];
            t.normal[2] = v.normal[2];
        }
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context,vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};

struct EllipseVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 1, float cof2 = 0, float cof3 = 0) override
    {
        Colour g = Colour(GRID_LINE);
        int n=180;
        int i;
        float rad = cof1;
        for(i=0 ; i<n ; i++)
        {
            buildArcLine(Vector(2*M_PI*(float)i/n, 0, rad), Vector(2*M_PI*(float)(i+1)/n, 0, rad), 1);
        }
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context, vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct FlatEllipseVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 1, float cof2 = 0, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(GRID_LINE);
        int n=18;
        int i;
        int bt, t, at;
        float rad = cof1;
        float ang;
        float thick = 0.2;
        Vector aed;
        for(i=0 ; i<n ; i++)
        {
            ang = 2*M_PI*(float)i/n;
            aed = getAED(Vector(rad * std::cos(ang), rad * std::sin(ang), -thick));
            vert = {
                {rad * std::cos(ang),rad * std::sin(ang),-thick},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector(rad * std::cos(ang),rad * std::sin(ang),thick));
            vert = {
                {rad * std::cos(ang),rad * std::sin(ang),thick},
                //        {0,0,0},
                //{aed.x, aed.y, aed.z},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector((rad-thick) * std::cos(ang),(rad-thick) * std::sin(ang), thick));
            vert = {
                {(rad-thick) * std::cos(ang),(rad-thick) * std::sin(ang), thick},
                //         {0,0,0},
                //{aed.x, aed.y, aed.z},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector((rad-thick) * std::cos(ang), (rad-thick) * std::sin(ang), -thick));
            vert = {
                {(rad-thick) * std::cos(ang), (rad-thick) * std::sin(ang), -thick},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            
            bt = i-1;
            t = i;
            at = i+1;
            if(bt<0)
                bt = n-1;
            if(at==n)
                at = 0;
            indecies.add(bt * 4);indecies.add(t * 4);indecies.add(t * 4 + 1);indecies.add(t * 4);indecies.add(t * 4 + 1);indecies.add(at * 4 + 1);
            
            indecies.add(bt * 4 + 1);indecies.add(t * 4 + 1);indecies.add(t * 4 + 2);indecies.add(t * 4 + 1);indecies.add(t * 4 + 2);indecies.add(at * 4 + 2);
            
            indecies.add(bt * 4 + 2);indecies.add(t * 4 + 2);indecies.add(t * 4 + 3);indecies.add(t * 4 + 2);indecies.add(t * 4 + 3);indecies.add(at * 4 + 3);
            
            indecies.add(bt * 4 + 3);indecies.add(t * 4 + 3);indecies.add(t * 4);indecies.add(t * 4 + 3);indecies.add(t * 4);indecies.add(at * 4);
        }
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context, vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct EllipseBandVertex : public ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1 = 1, float cof2 = 0, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(GRID_LINE);
        int n=30;
        int i;
        int bt, t, at;
        float rad = cof1;
        float ang;
        float thick = 0.012;
        Vector aed;
        for(i=0 ; i<n ; i++)
        {
            ang = 2*M_PI*(float)i/n;
            aed = getAED(Vector(rad * std::cos(ang), rad * std::sin(ang), -thick));
            vert = {
                {rad * std::cos(ang),rad * std::sin(ang),-thick},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector(rad * std::cos(ang),rad * std::sin(ang),thick));
            vert = {
                {rad * std::cos(ang),rad * std::sin(ang),thick},
        //        {0,0,0},
                //{aed.x, aed.y, aed.z},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector((rad-thick) * std::cos(ang),(rad-thick) * std::sin(ang), thick));
            vert = {
                {(rad-thick) * std::cos(ang),(rad-thick) * std::sin(ang), thick},
       //         {0,0,0},
                //{aed.x, aed.y, aed.z},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector((rad-thick) * std::cos(ang), (rad-thick) * std::sin(ang), -thick));
            vert = {
                {(rad-thick) * std::cos(ang), (rad-thick) * std::sin(ang), -thick},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            
            bt = i-1;
            t = i;
            at = i+1;
            if(bt<0)
                bt = n-1;
            if(at==n)
                at = 0;
            indecies.add(bt * 4);indecies.add(t * 4);indecies.add(t * 4 + 1);indecies.add(t * 4);indecies.add(t * 4 + 1);indecies.add(at * 4 + 1);
            
            indecies.add(bt * 4 + 1);indecies.add(t * 4 + 1);indecies.add(t * 4 + 2);indecies.add(t * 4 + 1);indecies.add(t * 4 + 2);indecies.add(at * 4 + 2);
            
            indecies.add(bt * 4 + 2);indecies.add(t * 4 + 2);indecies.add(t * 4 + 3);indecies.add(t * 4 + 2);indecies.add(t * 4 + 3);indecies.add(at * 4 + 3);
            
            indecies.add(bt * 4 + 3);indecies.add(t * 4 + 3);indecies.add(t * 4);indecies.add(t * 4 + 3);indecies.add(t * 4);indecies.add(at * 4);
        }
        for(i=n ; i<2*n ; i++)
        {
            ang = 2*M_PI*(float)i/n;
            aed = getAED(Vector(rad * std::cos(ang), -thick, rad * std::sin(ang)));
            vert = {
                {rad * std::cos(ang), -thick, rad * std::sin(ang)},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector(rad * std::cos(ang), thick, rad * std::sin(ang)));
            vert = {
                {rad * std::cos(ang), thick, rad * std::sin(ang)},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector((rad-thick) * std::cos(ang), thick, (rad-thick) * std::sin(ang)));
            vert = {
                {(rad-thick) * std::cos(ang), thick, (rad-thick) * std::sin(ang)},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector((rad-thick) * std::cos(ang), -thick, (rad-thick) * std::sin(ang)));
            vert = {
                {(rad-thick) * std::cos(ang), -thick, (rad-thick) * std::sin(ang)},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            
            bt = i-1;
            t = i;
            at = i+1;
            if(bt<n)
                bt = 2*n-1;
            if(at==2*n)
                at = n;
            indecies.add(bt * 4);indecies.add(t * 4);indecies.add(t * 4 + 1);indecies.add(t * 4);indecies.add(t * 4 + 1);indecies.add(at * 4 + 1);
            
            indecies.add(bt * 4 + 1);indecies.add(t * 4 + 1);indecies.add(t * 4 + 2);indecies.add(t * 4 + 1);indecies.add(t * 4 + 2);indecies.add(at * 4 + 2);
            
            indecies.add(bt * 4 + 2);indecies.add(t * 4 + 2);indecies.add(t * 4 + 3);indecies.add(t * 4 + 2);indecies.add(t * 4 + 3);indecies.add(at * 4 + 3);
            
            indecies.add(bt * 4 + 3);indecies.add(t * 4 + 3);indecies.add(t * 4);indecies.add(t * 4 + 3);indecies.add(t * 4);indecies.add(at * 4);
        }
        for(i=2*n ; i<3*n ; i++)
        {
            ang = 2*M_PI*(float)i/n;
            aed = getAED(Vector(-thick, rad * std::cos(ang),rad * std::sin(ang)));
            vert = {
                {-thick, rad * std::cos(ang),rad * std::sin(ang)},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector(thick, rad * std::cos(ang),rad * std::sin(ang)));
            vert = {
                {thick, rad * std::cos(ang),rad * std::sin(ang)},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector(thick, (rad-thick) * std::cos(ang), (rad-thick) * std::sin(ang)));
            vert = {
                {thick, (rad-thick) * std::cos(ang), (rad-thick) * std::sin(ang)},
                {std::sin(ang), std::cos(ang),0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            aed = getAED(Vector(-thick, (rad-thick) * std::cos(ang), (rad-thick) * std::sin(ang)));
            vert = {
                {-thick, (rad-thick) * std::cos(ang), (rad-thick) * std::sin(ang)},
                {std::sin(ang), std::cos(ang), 0},
                {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
                {0.5,0.5}};
            vertices.add(vert);
            
            bt = i-1;
            t = i;
            at = i+1;
            if(bt<2*n)
                bt = 3*n-1;
            if(at==3*n)
                at = 2*n;
            indecies.add(bt * 4);indecies.add(t * 4);indecies.add(t * 4 + 1);indecies.add(t * 4);indecies.add(t * 4 + 1);indecies.add(at * 4 + 1);
            
            indecies.add(bt * 4 + 1);indecies.add(t * 4 + 1);indecies.add(t * 4 + 2);indecies.add(t * 4 + 1);indecies.add(t * 4 + 2);indecies.add(at * 4 + 2);

            indecies.add(bt * 4 + 2);indecies.add(t * 4 + 2);indecies.add(t * 4 + 3);indecies.add(t * 4 + 2);indecies.add(t * 4 + 3);indecies.add(at * 4 + 3);

            indecies.add(bt * 4 + 3);indecies.add(t * 4 + 3);indecies.add(t * 4);indecies.add(t * 4 + 3);indecies.add(t * 4);indecies.add(at * 4);
        }
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context, vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct RectangleVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1=1, float cof2 = 1, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(GRID_LINE);
        vert = {
            {-cof1,cof2,0},
            {0,0,1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.0,1.0}};
        vertices.add(vert);
        vert = {
            {cof1,cof2,0},
            {0,0,1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {1.0,1.0}};
        vertices.add(vert);
        vert = {
            {cof1,-cof2,0},
            {0,0,1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {1.0,0.0}};
        vertices.add(vert);
        vert = {
            {-cof1,-cof2,0},
            {0,0,1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.0,0.0}};
        vertices.add(vert);
        indecies.add(0);
        indecies.add(1);
        indecies.add(2);
        indecies.add(0);
        indecies.add(2);
        indecies.add(3);
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context, vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct FloorVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1=5000, float cof2 = 5000, float cof3 = 0) override
    {
        Vertex vert;
        Colour g = Colour(RULER_LINE);
        vert = {
            {-cof1,0,cof2},
            {0,0,1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.0,1.0}};
        vertices.add(vert);
        vert = {
            {cof1,0,cof2},
            {0,0,1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {1.0,1.0}};
        vertices.add(vert);
        vert = {
            {cof1,0,-cof2},
            {0,0,1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {1.0,0.0}};
        vertices.add(vert);
        vert = {
            {-cof1,0,-cof2},
            {0,0,1},
            {g.getFloatRed(), g.getFloatGreen(), g.getFloatBlue(), g.getFloatAlpha()},
            {0.0,0.0}};
        vertices.add(vert);
        indecies.add(0);
        indecies.add(1);
        indecies.add(2);
        indecies.add(0);
        indecies.add(2);
        indecies.add(3);
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context, vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};
struct TextureVertex : ShapeVertex
{
    void initShape(OpenGLContext& context, float cof1=1, float cof2 = 0, float cof3 = 0) override
    {
        Vertex vert;
        Vector aed;
        aed = getAED(Vector(-cof1, cof1, 0));
        vert = {
            {-cof1,cof1,0},
            {0,0,1},
            {1.0,1.0,1.0,1.0},
            {0.0,1.0}};
        vertices.add(vert);
        vert = {
            {cof1,cof1,0},
            {0,0,1},
            {1.0,0.0,1.0,1.0},
            {1.0,1.0}};
        vertices.add(vert);
        aed = getAED(Vector(cof1, -cof1, 0));
        vert = {
            {cof1,-cof1,0},
            {0,0,1},
            {1.0,1.0,0.0,1.0},
            {1.0,0.0}};
        vertices.add(vert);
        aed = getAED(Vector(-cof1, -cof1, 0));
        vert = {
            {-cof1,-cof1,0},
            {0,0,1},
            {1.0,0.0,0.0,1.0},
            {0.0,0.0}};
        vertices.add(vert);
        indecies.add(0);
        indecies.add(1);
        indecies.add(2);
        indecies.add(0);
        indecies.add(2);
        indecies.add(3);
        context.extensions.glGenVertexArrays(1, &shapeVAO);
        context.extensions.glGenBuffers (1, &shapeVBO);
        context.extensions.glGenBuffers (1, &shapeEBO);
        context.extensions.glBindVertexArray(shapeVAO);
        putData(context, vertices, indecies);
        enableVertexArray(context);
        context.extensions.glBindVertexArray(0);
        vertexSize = indecies.size();
    }
};




#endif  // SHAPEVERTEX_H_INCLUDED
