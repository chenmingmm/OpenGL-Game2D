#include "Game.h"
#include "SpriteRenderer.h"
#include "resourcemanager.h"
#include "GameLevel.h"
#include "BallObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/glfw3.h>

SpriteRenderer  *Renderer;
SpriteRendererTest * Renderertest;

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

BallObject     *Ball; 

Game::Game(GLuint Width, GLuint height)
{
    this->Height = height;
    this->Width = Width;
    this->State = GAME_ACTIVE;
}
Game::~Game()
{
    delete Player;
}

void Game::Init()
{
 // 加载着色器
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    // 配置着色器
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), 
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // 设置专用于渲染的控制
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

     // 加载着色器
    ResourceManager::LoadShader("shaders/sprite_test.vs", "shaders/sprite_test.frag", nullptr, "spritetest");
    // 配置着色器
    projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), 
        0.0f,static_cast<GLfloat>(this->Height), -1.0f, 1.0f);
    ResourceManager::GetShader("spritetest").Use().SetInteger("image", 0);
    ResourceManager::GetShader("spritetest").SetMatrix4("projection", projection);
    // 设置专用于渲染的控制
    Renderertest = new SpriteRendererTest(ResourceManager::GetShader("spritetest"));
    // 加载纹理
    ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");

    ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
    // 加载关卡
    GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
    GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
    GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
    GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    glm::vec2 playerPos = glm::vec2(
        this->Width / 2 - PLAYER_SIZE.x / 2, 
        this->Height - PLAYER_SIZE.y
    );
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(glm::vec2(0.0f,0.0f), BALL_RADIUS, INITIAL_BALL_VELOCITY,
        ResourceManager::GetTexture("face"));
}

void Game::Render()
{
   if(this->State == GAME_ACTIVE)
    {
        //glViewport(0,0,400,600);
        // 绘制背景
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), 
            glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
        );
        // 绘制关卡
        this->Levels[this->Level].Draw(*Renderer);

        Player->Draw(*Renderer);

        //glViewport(400,0,400,600);

        Ball->Draw(*Renderertest);
    }
}

void Game::Update(GLfloat dt)
{
    //Ball->Move(dt, this->Width);
}

void Game::ProcessInput(GLfloat dt)
{
 if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // 移动挡板
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
    }
}