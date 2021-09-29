#include "SpriteRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void SpriteRenderer::DrawSprite(Texture &texture, glm::vec2 position, 
            glm::vec2 size, GLfloat rotate, 
            glm::vec3 color)
{
    // 准备变换
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f)); 

    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void SpriteRenderer::DrawSprite(Texture &&texture, glm::vec2 position, 
            glm::vec2 size, GLfloat rotate, 
            glm::vec3 color)
{
    DrawSprite(texture, position, size, rotate, color);
}

void SpriteRendererTest::DrawSprite(Texture &texture, glm::vec2 position, 
            glm::vec2 size, GLfloat rotate,
            glm::vec3 color)
{              
    // 准备变换
    this->shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f)); 

    model = glm::scale(model, glm::vec3(glm::vec2(100,100), 1.0f)); 
    this->shader.SetMatrix4("model", model);



    //glActiveTexture(GL_TEXTURE0);
    //texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
       // 配置 VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = { 
        // 位置     // 纹理
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

SpriteRenderer::SpriteRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::SpriteRenderer(Shader &&shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRendererTest::initRenderData()
{
   float vertices[] = {
     0.0f, 0.0f, 0.0f,
     1.0f, 0.0f, 0.0f,
     0.5f, 1.0f, 0.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0); 
}
