#include "robot.h"
#include "sim.h"
#include<vector>
#include<cmath>

// Constructor with default values
Robot::Robot()
    : InitialPosition(0.0f, 0.0f),
      CurrentPosition(0.0f, 0.0f),
      Velocity(100.0f, 100.0f),
      angularvelocity(50.0f),
      rotationangle(0.0f),
      Radius(RADIUS), 
      Sprite(ResourceManager::GetTexture("face")),
      isrotating(false)
{ }

// Constructor with specified values
Robot::Robot(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : InitialPosition(pos),
      CurrentPosition(pos),
      Velocity(velocity),
      Radius(radius),
      rotationangle(0.0f),
      Sprite(sprite),
      angularvelocity(100.0f),
      isrotating(false)
{}


void Robot::startpos(){
    //the parameter x is useless it is just used to overload the function
    int x = (Path[0][0] * 100.0f) + 50.0f - RADIUS;
    int y = (Path[0][1] * 100.0f) + 50.0f - RADIUS;

    InitialPosition = glm::vec2(x, y);
    CurrentPosition = InitialPosition;
    
    std::cout<<"Current position of the robot is: ("<<CurrentPosition.x<<" , "<<CurrentPosition.y<<")"<<"\n";
}


void Robot::rotateRobot(float dt, int x, int y){
 if(x == 1)
        targetangle =  90;  
    if(x == -1)
        targetangle =  -90;
    if(y == 1)
        targetangle = 180; 
    if(y == -1)
        targetangle = 0; 

    if(std::abs(this->rotationangle - targetangle) < 1.0f)
        this->isrotating = false;

    float direction = glm::normalize(targetangle - this->rotationangle);
    float rotation = direction * this->angularvelocity * dt;
    this->rotationangle += rotation;
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
