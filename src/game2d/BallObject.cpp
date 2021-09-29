
#include "BallObject.h"

void BallObject::Reset(glm::vec2 pos, glm::vec2 velocity)
{
    Position = pos;
    Velocity = velocity;
    this->Stuck = true;
}

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width)
{
    if (!this->Stuck)
    {
        Position += dt * Velocity;
        if (Position.x <= 0.0f) 
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0;
        }
        else if (this->Position.x + this->Size.x >= window_width)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = window_width - this->Size.x;
        }
        if (Position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }
    }
    return this->Position;
}

BallObject::BallObject() 
    : GameObject(), Radius(12.5f), Stuck(true)  { }

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture sprite)
    :  GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) { }