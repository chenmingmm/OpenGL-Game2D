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

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // 上
        glm::vec2(1.0f, 0.0f),  // 右
        glm::vec2(0.0f, -1.0f), // 下
        glm::vec2(-1.0f, 0.0f)  // 左
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}   

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
    // ResourceManager::LoadShader("shaders/sprite_test.vs", "shaders/sprite_test.frag", nullptr, "spritetest");
    // // 配置着色器
    // projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), 
    //     0.0f,static_cast<GLfloat>(this->Height), -1.0f, 100.0f);
    // ResourceManager::GetShader("spritetest").Use().SetInteger("image", 0);
    // ResourceManager::GetShader("spritetest").SetMatrix4("projection", projection);
    // // 设置专用于渲染的控制
    // Renderertest = new SpriteRendererTest(ResourceManager::GetShader("spritetest"));
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
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
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

        Ball->Draw(*Renderer);
    }
}

void Game::Update(GLfloat dt)
{
    Ball->Move(dt, this->Width);

    this->DoCollision();
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
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                {
                    Ball->Position.x -= velocity;
                }  
            }               
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                if (Ball->Stuck){
                    Ball->Position.x += velocity;
                }
               
            }              
        }

        if (this->Keys[GLFW_KEY_SPACE])
        {
            Ball->Stuck = false;
        }
    }
}

GLboolean Game::checkCollision(GameObject &one, GameObject &two)
{
    bool collision_x = one.Position.x + one.Size.x >= two.Position.x &&
    two.Position.x + two.Size.x >= one.Position.x;

    bool collision_y = one.Position.y + one.Size.y >= two.Position.y &&
    two.Position.y + two.Size.y >= one.Position.y;

    return collision_x && collision_y;   
}

GLboolean Game::checkCollision(BallObject &one, GameObject &two)
{
    // 获取圆的中心 
    glm::vec2 center(one.Position + one.Radius);
    // 计算AABB的信息（中心、半边长）
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x, 
        two.Position.y + aabb_half_extents.y
    );
    // 获取两个中心的差矢量
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
    glm::vec2 closest = aabb_center + clamped;
    // 获得圆心center和最近点closest的矢量并判断是否 length <= radius
    difference = closest - center;
    return glm::length(difference) < one.Radius;
}

void Game::DoCollision()
{
    for (auto &iter : this->Levels[this->Level].Bricks)
    {
        if (iter.Destroyed){
            continue;
        }
        if (checkCollision(*Ball, iter)){

            if (!iter.IsSolid) {
                 iter.Destroyed = true;
            }          
        }
    }
}


