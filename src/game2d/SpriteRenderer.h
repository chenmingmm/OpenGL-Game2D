#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "../shaderManager.h"
#include "texture.h"

class SpriteRenderer
{
public:
    SpriteRenderer(){}
    SpriteRenderer(Shader &shader);
    SpriteRenderer(Shader &&shader);
    virtual ~SpriteRenderer();

    virtual void DrawSprite(Texture &texture, glm::vec2 position, 
            glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, 
            glm::vec3 color = glm::vec3(1.0f));
    
    void DrawSprite(Texture &&texture, glm::vec2 position, 
        glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, 
        glm::vec3 color = glm::vec3(1.0f));

    void RenderTime(GLfloat dt);

protected:
    Shader shader; 
    GLuint quadVAO;

    virtual void initRenderData();
};

class SpriteRendererTest : public SpriteRenderer
{
private:
    /* data */
public:
    SpriteRendererTest(Shader &shader)
    {
        this->shader = shader;
        initRenderData();
    }

    SpriteRendererTest(Shader &&shader)
    {
        this->shader = shader;
        initRenderData();
    }

    virtual void DrawSprite(Texture &texture, glm::vec2 position, 
            glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, 
            glm::vec3 color = glm::vec3(1.0f))override;

    ~SpriteRendererTest(){}

    virtual void initRenderData()override;
};


#endif
