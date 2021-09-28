#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include "GameLevel.h"

enum GameState{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
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
};

#endif