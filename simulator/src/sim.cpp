#include "sim.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "sim_object.h"
#include "robot.h"
#include <iostream>
#include <cmath>

// Game-related State data
SpriteRenderer  *Renderer, *renderer2;
std::vector<glm::vec2> InitialPositions;

Sim::Sim(unsigned int width, unsigned int height) 
    : Width(width), Height(height)
{ 

}

Sim::~Sim()
{
    delete Renderer;
    for (auto robot : Robots) {
        delete robot;
    }
    Robots.clear();
}

std::map<Direction, float> directionAngles = {
    {UP, 0.0f},
    {DOWN, 180.0f},
    {LEFT, -90.0f},
    {RIGHT, 90.0f}
};

void Sim::Init()
{
    // load shaders
    ResourceManager::LoadShader("C:/Users/Lenovo/Desktop/simulator/shaders/sprite.vs", "C:/Users/Lenovo/Desktop/simulator/shaders/sprite.fs", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), 
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    renderer2 = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    
    // load textures
    ResourceManager::LoadTexture("C:/Users/Lenovo/Desktop/simulator/textures/background.jpg", false, "background");
    ResourceManager::LoadTexture("C:/Users/Lenovo/Desktop/simulator/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("C:/Users/Lenovo/Desktop/simulator/textures/block.png", false, "block");

    // load levels
    Level.Load("C:/Users/Lenovo/Desktop/simulator/levels/one.lvl", this->Width, this->Height);

    InitialPositions = {
        glm::vec2((0.0f * 160.0f) + 80 - RADIUS, (0.0f * 120.0f) + 60.0f - RADIUS),
        glm::vec2((2.0f * 160.0f) + 80 - RADIUS, (0.0f * 120.0f) + 60.0f - RADIUS),
        glm::vec2((2.0f * 160.0f) + 80 - RADIUS, (4.0f * 120.0f) + 60.0f - RADIUS),
    };

    // Configure game objects
    Robots.push_back(new Robot(InitialPositions[0], RADIUS, INITIAL_VELOCITY, ResourceManager::GetTexture("face")));
    Robots[0]->Path = {
        {0, 0}, {0, 1}, {1, 1}, {2 , 1}, {2, 0}, {1, 0}, {0, 0}
    };

    Robots.push_back(new Robot(InitialPositions[1], RADIUS, INITIAL_VELOCITY, ResourceManager::GetTexture("face")));
    Robots[1]->Path = {
        {2, 0}, {2, 1}, {3, 1}
    };

    Robots.push_back(new Robot(InitialPositions[2], RADIUS, INITIAL_VELOCITY, ResourceManager::GetTexture("face")));
    Robots[2]->Path = {
        {2, 4}, {3, 4 }, {3, 3}
    };
}

void Sim::Update(float dt)
{  
    // Update each robot
    for (auto robot : Robots) {
        
        if (robot->currentPathIndex < robot->Path.size()) {
            int x = robot->Path[robot->currentPathIndex][0] - robot->Path[robot->currentPathIndex - 1][0];
            int y = robot->Path[robot->currentPathIndex][1] - robot->Path[robot->currentPathIndex - 1][1];

            glm::vec2 targetPosition = glm::vec2(robot->InitialPosition.x + (x * 160.0f), robot->InitialPosition.y + (y * 120.0f));
            glm::vec2 currentPosition = robot->GetPosition();

            // Check if the robot has reached the target position
            if (glm::distance(currentPosition, targetPosition) < 1.0f) {
                // Move to the next target position
                robot->InitialPosition = currentPosition;
                robot->currentPathIndex++;
            }
            // Update the robot's position
            robot->Move(dt,x , y, targetPosition);
            robot->calculateDirection(x,y);
        }
    }

    // Other game update logic...
}


void Sim::Render()
{
    // Draw background, levels, and all robots
    Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
    this->Level.Draw(*Renderer);

    for (auto robot : Robots) {
        robot->Draw(*renderer2); 
    }
}

