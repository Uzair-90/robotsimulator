#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "grid_level.h"
#include "robot.h"

const glm::vec2 INITIAL_VELOCITY(50.0f, 50.0f);

const float RADIUS = 30.0f;

class Sim
{
public:
    // game state
    unsigned int Width, Height;
    Grid  Level;  
    // constructor/destructor
    Sim(unsigned int width, unsigned int height);
    ~Sim();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void Update(float dt);
    void Render();

private:
    std::vector<Robot*> Robots;
    //float lastRotationTime;
};

#endif
