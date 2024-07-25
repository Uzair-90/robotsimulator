#include "robot.h"
#include "sim.h"
#include<windows.h>

// Constructor with default values
Robot::Robot()
    : InitialPosition(0.0f, 0.0f), CurrentPosition(0.0f, 0.0f), Velocity(100.0f, 100.0f),Direction(0.0f,0.0f),rotationangle(0.0f),rotationcounter(0), Radius(1.0f), Sprite()
{ }

// Constructor with specified values
Robot::Robot(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : InitialPosition(pos), CurrentPosition(pos), Velocity(velocity), Radius(radius),rotationangle(0.0f), Sprite(sprite)
{}

void Robot::startpos(){
    if(!Path.empty()){
        int x = (this->Path[0][0] * 160.0f) + 80.0f - RADIUS;
        int y = (this->Path[0][1] * 120.0f) + 60.0f - RADIUS;
        InitialPosition = glm::vec2(x,y);
    }
}

void Robot::calculateDirection(int x, int y){
    if(x == -1){ //up 
        rotationangle = -90.0f;
    } else if(x == 1){ //down
        rotationangle = 90.0f;
    } else if (y == -1 ){ //right
        rotationangle = 0.0f;
    } else if (y == 1){ //LEFT
        rotationangle = 180.0f;
    }
}

// Function to move the ball
void Robot::Move(float dt, int x, int y, glm::vec2 targetPosition)
{
    
    glm::vec2 velocity = this->Velocity;
    velocity.x = x * velocity.x;
    velocity.y = y * velocity.y;
    // Calculate movement amount
    glm::vec2 movement = velocity * dt;

    if(x > 0){
        if(CurrentPosition.x + movement.x > targetPosition.x) {CurrentPosition.x = targetPosition.x; movement.x = 0;}
    }
    else if(x < 0){
        if(CurrentPosition.x + movement.x < targetPosition.x) {CurrentPosition.x = targetPosition.x; movement.x = 0;}
    }
    if(y > 0){
        if(CurrentPosition.y + movement.y > targetPosition.y) {CurrentPosition.y = targetPosition.y; movement.y = 0;}
    }
    else if(y < 0){
        if(CurrentPosition.y + movement.y < targetPosition.y) {CurrentPosition.y = targetPosition.y; movement.y = 0;}
    }
    // Update the ball's CurrentPosition
    CurrentPosition += movement;
}



// Function to reset the ball's CurrentPosition and velocity
void Robot::Reset(glm::vec2 CurrentPosition, glm::vec2 velocity)
{
    CurrentPosition = CurrentPosition;
    Velocity = velocity;
}

// Function to draw the ball
void Robot::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(Sprite, CurrentPosition, glm::vec2(Radius * 2, Radius * 2), rotationangle, glm::vec3(1.0f));
}

// Getter function for retrieving the ball's CurrentPosition
glm::vec2 Robot::GetPosition()
{
    return CurrentPosition;
}
