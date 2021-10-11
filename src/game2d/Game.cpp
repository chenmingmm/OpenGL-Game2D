#include "Game.h"
#include "SpriteRenderer.h"
#include "resourcemanager.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticalGenerator.h"
#include "PostProcessor.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw/glfw3.h>

SpriteRenderer  *Renderer;
SpriteRendererTest * Renderertest;
ParticleGenerator *particleGenerator;
PostProcessor *Effects;

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

GLfloat ShakeTime = 0.0f;

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
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.frag", nullptr, "particle");
    ResourceManager::LoadShader("shaders/postprocessor.vs", "shaders/postprocessor.frag", nullptr, "Effects");
    // 配置着色器
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), 
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
    // 设置专用于渲染的控制
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // 加载纹理
    ResourceManager::LoadTexture("textures/particle.png", GL_TRUE, "particle"); 
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
    Ball = new BallObject(ballPos, BALL_RADIUS, glm::vec2(0.0f,0.0f),
        ResourceManager::GetTexture("face"));

    particleGenerator = new ParticleGenerator(
        ResourceManager::GetShader("particle"), 
        ResourceManager::GetTexture("particle"), 
        500
    );

    Effects = new PostProcessor(ResourceManager::GetShader("Effects"), Width, Height);
}

void Game::Render()
{
   if(this->State == GAME_ACTIVE)
    {
        Effects->BeginRender();
        // 绘制背景
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), 
            glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
        );
        // 绘制关卡
        this->Levels[this->Level].Draw(*Renderer);

        Player->Draw(*Renderer);

        particleGenerator->Draw();

        Ball->Draw(*Renderer);

        Effects->EndRender();
        Effects->Render(glfwGetTime());
    }
}

void Game::Update(GLfloat dt)
{
    Ball->Move(dt, this->Width);

    particleGenerator->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));

    this->DoCollision();

    if (Ball->Position.y >= this->Height) // 球是否接触底部边界？
    {
        //this->ResetLevel();
        this->ResetPlayer();
    }

    if (ShakeTime > 0.0f) {
        ShakeTime -= dt;
        if ( ShakeTime <= 0.0f) {
            Effects->Shake = false;
        }
    }
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
            Ball->Velocity = INITIAL_BALL_VELOCITY;
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

CollisionData Game::checkCollision(BallObject &one, GameObject &two)
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
    //return glm::length(difference) < one.Radius;
    if (glm::length(difference) < one.Radius) {
        return CollisionData(GL_TRUE, VectorDirection(difference), difference);
    } else {
        return CollisionData(GL_FALSE, UP, glm::vec2(0.0f,0.0f));
    }
}

void Game::DoCollision()
{
    for (auto &iter : this->Levels[this->Level].Bricks)
    {
        if (iter.Destroyed){
            continue;
        }
        CollisionData data = checkCollision(*Ball, iter);
        if (data.isCollision){
            if (!iter.IsSolid) {
                 iter.Destroyed = true;
            } else {
                Effects->Shake = true;
                ShakeTime = 0.05f;
            }
            if (data.dir == LEFT || data.dir == RIGHT) {
                Ball->Velocity.x = -Ball->Velocity.x;
                GLfloat penetration = Ball->Radius - glm::abs(data.diff_vector.x);
                if (data.dir == LEFT) {
                    Ball->Position.x += penetration;
                } else {
                    Ball->Position.y -= penetration;
                }
            }
            else 
            {
                Ball->Velocity.y = -Ball->Velocity.y;
                GLfloat penetration = Ball->Radius - glm::abs(data.diff_vector.y);
                if (data.dir == UP)
                {
                    Ball->Position.y -= penetration;
                }
                else
                {
                    Ball->Position.y += penetration;
                }
            }
        }
    }
    CollisionData playerdata = checkCollision(*Ball, *Player);
    if (!Ball->Stuck && playerdata.isCollision) {
                // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);
        // 依据结果移动
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength; 
        Ball->Velocity.y = -1 * glm::abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
}

void Game::ResetLevel()
{
    if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 1)
        this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 2)
        this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 3)
        this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}


