#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#pragma once

#include <glm/glm.hpp>
#include "texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
    glm::vec2 Position;
    glm::vec2 Size;
    glm::vec2 Velocity;
    glm::vec3 Color;
    GLfloat Rotation;
    GLboolean IsSolid;
    GLboolean Destroyed;
    Texture Sprite;
    /* data */
public:
    GameObject();
    virtual ~GameObject() {}

    GameObject(glm::vec2 pos, glm::vec2 size, Texture &sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    virtual void Draw(SpriteRenderer &renderer);
};


#endif