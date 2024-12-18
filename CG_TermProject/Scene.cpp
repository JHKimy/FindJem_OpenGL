#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Actor.h"
#include "Character.h"

#include "MazeGenerator.h"
#include "Controller.h"

// �� ����
Scene::Scene(GLuint shaderProgram) 
    :   SceneShader(shaderProgram)
{
    // ī�޶� ����
    mainCamera = make_unique<Camera>(shaderProgram);
    // ����Ʈ ���� (�� ���ӿ��� �ϳ��� �ʿ�)
    mainLight = make_unique<Light>(
        glm::vec3(0.0f, 10.0f, 10.0f),  // pos
        glm::vec3(1.0f));               // color

    // �̷� ������
    mazeGenerator = make_unique<MazeGenerator>(15, 15);
}

// �� �ʱ�ȭ
void Scene::Initialize()
{
    // MazeGenerator�� ���� �̷� ����
    mazeGenerator->GeneratePrimMaze();
    // �Ա� �ձ�
    mazeGenerator->addEntrances();
    // �̷� ������ (0�� 1)
    mazeMap = mazeGenerator->GetMaze();

    // ������ �ؾ��� actors�� ť�����·� �߰�
    InitializeMaze();

    // ó������ �� ž��
    mainCamera->TopView();

    // �÷��̾� ĳ���� �ʱ�ȭ
    mainCharacter = make_unique<Character>
        (glm::vec3(-3.f, 0.f, -3.f));// Position                    
}

void Scene::Update(float deltaTime)
{
    // �÷��̾� ���� ������Ʈ
    mainCharacter->Update(deltaTime);
}

void Scene::Render()
{
    // ī�޶�� Controller���� ����
    
    // ���� ����
    mainLight->ApplyLighting(SceneShader, mainCamera->GetPosition());

    // �÷��̾� ������
    mainCharacter->Render(SceneShader);

    // Actor ������
    for (const auto& actor : actors) {
        actor->Render(SceneShader);
    }
}

Character* Scene::GetCharacter()
{
    return mainCharacter.get();
}

Camera* Scene::GetCamera()
{
    return mainCamera.get();
}

const std::vector<std::unique_ptr<Actor>>& Scene::GetActors() const
{
    return actors;
}

void Scene::InitializeMaze()
{
    // �̷��� �� 1�� ũ��
    glm::vec3 blockSize(5.f, 3.f, 5.f);

    for (int y = 0; y < mazeMap.size(); ++y) 
    {
        for (int x = 0; x < mazeMap[y].size(); ++x) 
        {
            if (mazeMap[y][x] == 1) {
                actors.push_back(make_unique<Actor>(
                    "Cube.obj",           
                    glm::vec3(x * blockSize.x, 0.0f, y * blockSize.z), // Position
                    glm::vec3(blockSize),   // ũ��
                    glm::vec3(0),           // ȸ����
                    glm::vec3(0, 0, 1)));   // ����
            }
        }
    }
}