#ifndef POWERUP_H
#define POWERUP_H

#include "GameObject.h"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
    std::string Itemtype;
    GLfloat     Duration;   
    GLboolean   Activated;
    // 构造函数
    PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture texture) 
        :GameObject(position,SIZE, texture, color, VELOCITY), Itemtype(type), Duration(duration), Activated(false) 
    { }
    ~PowerUp() {};
};


#endif