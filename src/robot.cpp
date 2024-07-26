#include "robot.h"
#include "AStar.hpp"
#include "sim.h"
#include<vector>
#include<cmath>

// Constructor with default values
Robot::Robot()
    : InitialPosition(0.0f, 0.0f),
      CurrentPosition(0.0f, 0.0f),
      Velocity(100.0f, 100.0f),
      angularvelocity(70.0f),
      rotationangle(0.0f),
      Radius(RADIUS), 
      Sprite(ResourceManager::GetTexture("face")),
      isrotating(true)
{ }

// Constructor with specified values
Robot::Robot(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
    : InitialPosition(pos),
      CurrentPosition(pos),
      Velocity(velocity),
      Radius(radius),
      rotationangle(0.0f),
      Sprite(sprite),
      angularvelocity(50.0f),
      isrotating(true)
{}

void Robot::findPath(std::pair<int, int> start, std::pair<int, int> goal){
    //set the start and goal for the grid
    AStar::Vec2i vec1 {start.first, start.second};
    AStar::Vec2i vec2 {goal.first, goal.second};

    //set the grid size in our case it is 5x5 
    generator.setWorldSize({5,5});
    generator.setHeuristic(AStar::Heuristic::manhattan);
    generator.setDiagonalMovement(false);

    auto reversedpath = generator.findPath(vec1,vec2);

    for(auto it = reversedpath.rbegin(); it != reversedpath.rend(); ++it) {
        const auto& item = *it;
        Path.push_back({item.x, item.y});
    }

}


void Robot::startpos(){
    if(!Path.empty()){
        int x = (Path[0][0] * 160.0f) + 80.0f - RADIUS;
        //std::cout<<"X: "<<x<<"\n";
        int y = (Path[0][1] * 120.0f) + 60.0f - RADIUS;
        //std::cout<<"Y: "<<y<<"\n";
        InitialPosition = glm::vec2(x,y);
        CurrentPosition = InitialPosition;
    }
}


void Robot::rotateRobot(float dt, int x, int y){
    if(x == -1){ //up 
            targetangle = -90.0f;
        } else if(x == 1){ //down
            targetangle = 90.0f;
        } else if (y == -1 ){ //right
            targetangle = 0.0f;
        } else if (y == 1){ //LEFT
            targetangle = 180.0f;
        }

        if(targetangle >= 360.0){
            targetangle = 0.0f;
        }

        float direction = glm::normalize(targetangle - rotationangle);
    
        rotationangle += direction * dt * angularvelocity; 

        if(std::abs(rotationangle - targetangle) < 1.0f){
            isrotating = false;
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
