#include "Scene.h"

Scene::Scene(GLuint shaderProgram): 
    mainCamera(
        glm::vec3(10.0f, 10.0f, 10.0f), // pos
        glm::vec3(0.0f),                // target
        glm::vec3(0.0f, 1.0f, 0.0f)),   // up
    mainLight(
        glm::vec3(0.0f, 10.0f, 10.0f),  // pos
        glm::vec3(1.0f)),               // color
    shaderProgram(shaderProgram) {}

void Scene::Initialize()
{
    // Actor 및 Character 초기화
    actors.push_back(new Actor("Cube.obj", glm::vec3(0), glm::vec3(1), glm::vec3(0), glm::vec3(1, 0, 0)));
    player = new Character("Boss.obj", glm::vec3(2.f, 0.f, 0.f), glm::vec3(.1f), glm::vec3(0), glm::vec3(0, 1, 0), 0.05f, 100);
}

void Scene::Update(float deltaTime)
{
}

void Scene::Render()
{
}

void Scene::Shutdown()
{
}
