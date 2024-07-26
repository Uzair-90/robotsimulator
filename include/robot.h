#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>
#include "texture.h"
#include "sprite_renderer.h"
#include "AStar.hpp"
#include <vector>
#include<map>

// Defines a Ball object with relevant properties and methods
class Robot
{
public:
    // Ball state
    glm::vec2   InitialPosition, CurrentPosition, Velocity, Direction;
    float       Radius;
    float rotationangle;
    bool isrotating = true;
    float targetangle = 0;
    float angularvelocity;
    Texture2D   Sprite;
    AStar::Generator generator;
    std::vector<std::vector<int>> Path;
    unsigned int currentPathIndex = 1;

    // Constructor
    Robot();
    Robot(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

    // Functions
    void Move(float dt, int x, int y, glm::vec2 targetPosition);
    void Reset(glm::vec2 position, glm::vec2 velocity);
    void Draw(SpriteRenderer &renderer);
    void startpos();
    void rotateRobot(float dt, int x, int y);
    void findPath(std::pair<int, int> start, std::pair<int, int> goal);

    // Getter functions
    glm::vec2 GetPosition();
};

#endif
