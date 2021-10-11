#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#pragma once

#include "../shaderManager.h"
#include "texture.h"


class PostProcessor
{
public:
    PostProcessor(/* args */);
    PostProcessor(Shader &, GLuint, GLuint);
    ~PostProcessor();

    void BeginRender();
    void EndRender();
    void Render(GLfloat time);
    void InitRenderData();

    bool Confuse;
    bool Chaos;
    bool Shake;

    Shader PostProcessorshader;
    Texture texture;
    GLuint Width;
    GLuint Height;

    GLuint MSFBO, FBO; // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer to texture
    GLuint RBO; // RBO is used for multisampled color buffer
    GLuint VAO;
};


#endif