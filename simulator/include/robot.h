#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdio.h>
#include "texture.h"
#include "sprite_renderer.h"
#include <vector>
#include<map>


enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

extern std::map<Direction, float> directionAngle;


// Defines a Ball object with relevant properties and methods
class Robot
{
public:
    // Ball state
    glm::vec2   InitialPosition, CurrentPosition, Velocity, Direction;
    float       Radius;
    enum Direction currentDirection;
    float rotationangle;
    int rotationcounter;
    Texture2D   Sprite;
    std::vector<std::vector<unsigned int>> Path;
    unsigned int currentPathIndex = 1;

    // Constructor
    Robot();
    Robot(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

    // Functions
    void Move(float dt, int x, int y, glm::vec2 targetPosition);
    void Reset(glm::vec2 position, glm::vec2 velocity);
    void Rotaterobot(float angle, SpriteRenderer &renderer);
    void Draw(SpriteRenderer &renderer);
    void startpos();
    void calculateDirection(int x, int y);

    // Getter functions
    glm::vec2 GetPosition();
};

#endif
