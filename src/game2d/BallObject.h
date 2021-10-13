#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#pragma once

#include "GameObject.h"

class BallObject : public GameObject
{

public:
    GLfloat Radius;
    GLboolean Stuck;
    GLboolean Sticky = GL_FALSE;
    GLboolean PassThrough = GL_FALSE;

    void Reset(glm::vec2 pos, glm::vec2 velocity);

    glm::vec2 Move(GLfloat dt, GLuint window_width);


    BallObject(/* args */);
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture sprite);
    virtual ~BallObject() {}
};



#endif