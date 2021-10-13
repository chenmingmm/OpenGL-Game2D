#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <tuple>
#include <vector>
#include "GameLevel.h"
#include "BallObject.h"
#include "PowerUp.h"

enum GameState{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct CollisionData{
    GLboolean isCollision;
    Direction dir;
    glm::vec2 diff_vector;

    CollisionData(GLboolean b, Direction d, glm::vec2 vec2):
    isCollision(b),dir(d),diff_vector(vec2)
    {}
};

class Game
{

public:
    GameState State;
    GLboolean Keys[1024];
    GLuint Width;
    GLuint Height;
    std::vector<GameLevel> Levels;
    GLuint                 Level;
    GameObject      *Player;
    std::vector<PowerUp>  PowerUps;

public:
    Game(GLuint Width, GLuint height);
    ~Game();

    void Init();
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    void DoCollision();
    void ResetLevel();
    void ResetPlayer();
    void SpawnPowerUps(GameObject &block);
    void UpdatePowerUps(GLfloat dt);
    void ActivatePowerUp(PowerUp &powerup);
    GLboolean checkCollision(GameObject &one, GameObject &two);
    CollisionData checkCollision(BallObject &ball, GameObject &two);
};

#endif