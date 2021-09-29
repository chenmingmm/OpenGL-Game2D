#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <tuple>
#include "GameLevel.h"
#include "BallObject.h"

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

public:
    Game(GLuint Width, GLuint height);
    ~Game();

    void Init();
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    void DoCollision();
    GLboolean checkCollision(GameObject &one, GameObject &two);
    GLboolean checkCollision(BallObject &ball, GameObject &two);
};

#endif