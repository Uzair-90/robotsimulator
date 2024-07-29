#include "sim.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "sim_object.h"
#include "robot.h"
#include <iostream>
#include <cmath>
#include "jointastar.h"

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

    for(int i = 0; i < 4; i++){
        Robots.push_back(new Robot());
    }

    int n = 6;  // Grid size
    vector<Point> starts = {{2, 3}, {2, 1}, {3, 4}, {2,5}};
    vector<Point> goals = {{4, 2}, {4, 4}, {4, 1}, {0, 0}};

    vector<vector<Point>> path = jointStateAStar(starts, goals, n);

    // Print the found path or indicate no path found
    if (!path.empty()) {
        cout << "Path found:" << endl;

        // Vector to store each column as a separate path
        vector<vector<Point>> agentPaths(path[0].size());

        for (const auto& state : path) {
            for (size_t col = 0; col < state.size(); ++col) {
                agentPaths[col].push_back(state[col]);
            }
        }

        // Display each separated path (each column) horizontally
        cout << "\nAgent paths (each column):" << endl;
        for (size_t col = 0; col < agentPaths.size(); ++col) {
            cout << "Agent " << col << ": ";
            for (const auto& pos : agentPaths[col]) {
                cout << "(" << pos.x << "," << pos.y << ") ";
                if (col < Robots.size()) {
                    cout<<"Pushing path for robot: "<<col<<"\n";
                    Robots[col]->Path.push_back({pos.x, pos.y});
                }
            }
            cout << endl;
        }

        int x = 0;
        // Call startpos for each robot
        for (auto& robot : Robots) {
            if (robot) {
                robot->startpos();
                cout<<"Calculating stating position for robot: "<< x <<"\n";
                x++;
            }
        }
    } else {
        cout << "No path found." << endl;
    }


}

void Sim::Update(float dt)
{
    //int i = 0;
    // Update each robot
    for (auto robot : Robots) {

        //cout<<"Starting simulation for robot: "<<i<<"\n";
        //i++;

        if (robot->currentPathIndex < robot->Path.size()) { // Ensure there is a next point in the path

            int x = robot->Path[robot->currentPathIndex ][0] - robot->Path[robot->currentPathIndex - 1][0];
            int y = robot->Path[robot->currentPathIndex ][1] - robot->Path[robot->currentPathIndex - 1][1];

            robot->rotateRobot(dt, x, y);

            if (!robot->isrotating)
            {
                glm::vec2 targetPosition = glm::vec2(robot->InitialPosition.x + (x * 100.0f), robot->InitialPosition.y + (y * 100.0f));
                glm::vec2 currentPosition = robot->GetPosition();

                // Check if the robot has reached the target position
                if (glm::distance(currentPosition, targetPosition) < 1.0f) {
                    // Move to the next target position
                    robot->isrotating = true;
                    robot->InitialPosition = currentPosition;
                    robot->currentPathIndex++;
                } 
                    // Update the robot's position
                robot->Move(dt, x, y, targetPosition);
                
            }
        }
    }
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

