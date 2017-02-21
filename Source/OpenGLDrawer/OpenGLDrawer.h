//
//  OpenGLDrawer.h
//  GaudiReaper
//
//  Created by Ricky on 2016. 8. 13..
//
//

#ifndef OpenGLDRAWER_h
#define OpenGLDRAWER_h

#include "../JuceLibraryCode/JuceHeader.h"

#include "Shape.h"

class OpenGLDrawer
{
public:
    OpenGLDrawer(OpenGLContext& openGLContext,
                    int screenWidth,
                    int screenHeight,
                    int renderTargetNum) :
    context(openGLContext),
    width(screenWidth),
    height(screenHeight),
    outputTextureNum(renderTargetNum){}
    OpenGLShaderProgram::Uniform* createUniform (const char* uniformName)
    {
        if (context.extensions.glGetUniformLocation (shaderProgram->getProgramID(), uniformName) < 0)
            return nullptr;
        return new OpenGLShaderProgram::Uniform (*shaderProgram, uniformName);
    }
    void initializeShader(const char* vs, const char* fs)
    {
        String statusText;
        shaderProgram = new OpenGLShaderProgram(context);
        if (shaderProgram->addVertexShader (vs)
            && shaderProgram->addFragmentShader (fs)
            && shaderProgram->link())
        {
            statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
        }
        else
        {
            statusText = shaderProgram->getLastError();
        }
    }
    void genFrameBuffer()
    {
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        outputTexture.resize(outputTextureNum);
        glGenTextures(outputTextureNum, outputTexture.getRawDataPointer());
        for (GLuint i = 0; i < outputTextureNum ; i++)
        {
            glBindTexture(GL_TEXTURE_2D, outputTexture[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            context.extensions.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, outputTexture[i], 0);
        }
        context.extensions.glGenRenderbuffers (1, &renderBuffer);
        context.extensions.glBindRenderbuffer (GL_RENDERBUFFER, renderBuffer);
        context.extensions.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        context.extensions.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
        GLuint* attachments = new GLuint[outputTextureNum];
        for(GLuint i=0 ; i<outputTextureNum ; i++)
        {
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(outputTextureNum, attachments);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        delete[] attachments;
    }
    GLuint getTexture(int idx)
    {
        return outputTexture[idx];
    }
    virtual void draw() = 0;
    virtual ~OpenGLDrawer()
    {
        context.extensions.glDeleteRenderbuffers(1, &renderBuffer);
        context.extensions.glDeleteFramebuffers(1, &frameBuffer);
    }
protected:
    OpenGLContext& context;
    ScopedPointer<OpenGLShaderProgram> shaderProgram;
    GLuint frameBuffer, renderBuffer;
    virtual const char* getVertexShader() = 0;
    virtual const char* getFragmentShader() = 0;
    
    const int width;
    const int height;
    
    Array<GLuint> outputTexture;
    const int outputTextureNum;
};


#endif /* OpenGLDrawer_h */
