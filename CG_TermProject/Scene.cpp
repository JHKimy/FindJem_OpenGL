#include "Scene.h"
#include "Controller.h"

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

    // Actor �ʱ�ȭ
    actors.push_back(new Actor(
        "Cube.obj",             // filePath
        glm::vec3(1,1,1),       // Position
        glm::vec3(1),           // Scale
        glm::vec3(0),           // Rotation
        glm::vec3(1, 0, 0)));   // Color 
    
    mainCharacter = new Character(
        "Boss.obj",              // filePath
        glm::vec3(2.f, 0.f, 0.f),// Position,
        glm::vec3(.1f),          // Scale
        glm::vec3(0),            // Rotation
        glm::vec3(0, 1, 0),      // Color 
        0.05f,                   // speed
        100);                    // health

}

void Scene::Update(float deltaTime)
{

    // �÷��̾� ���� ������Ʈ
    mainCharacter->Update(deltaTime);
    

}

void Scene::Render()
{
    if (Command[Num1]) {
        mainCamera.SwitchToFirstPerson(
            mainCharacter->GetPosition(), mainCharacter->GetDirection());
    }
    if (Command[Num2]) {
        mainCamera.SwitchToTopView(mainCharacter->GetPosition());
    }

    // ī�޶� ����
    mainCamera.ApplyCamera(shaderProgram);

    // ���� ����
    mainLight.ApplyLighting(shaderProgram, mainCamera.GetPosition());

    // �÷��̾� ������
    mainCharacter->Render(shaderProgram);

    for (Actor* actor : actors) {
        actor->Render(shaderProgram);
    }
}

void Scene::Shutdown()
{
}

Character* Scene::GetCharacter()
{
    return mainCharacter;
}

Camera* Scene::GetCamera()
{
    return &mainCamera;
}