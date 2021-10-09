#ifndef PARTICAL_GENERATOR_H
#define PARTICAL_GENERATOR_H

#pragma once

#include<vector>

#include "../shaderManager.h"
#include "texture.h"
#include "GameObject.h"

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;

    Particle() 
      : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleGenerator
{

public:
    ParticleGenerator(Shader, Texture, GLuint);
    void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f,0.0f));
    void Draw();

private:
    std::vector<Particle> particles;
    GLuint amount;
    Shader shader;
    Texture texture;
    GLuint VAO;

    void Init();
    GLuint firstUnusedParticle();
    void respawnParticle(Particle&, GameObject&, glm::vec2 offset = glm::vec2(0.0f,0.0f));
};


#endif