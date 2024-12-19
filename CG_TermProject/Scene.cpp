#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Actor.h"
#include "Character.h"
#include"Bullet.h"
#include "Enemy.h"

#include "MazeGenerator.h"
#include "Controller.h"
#include <random>

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

    // �̷� ť�� 1���� ũ��
    blockSize = glm::vec3(5.f, 3.f, 5.f);
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



    // �÷��̾� ĳ���� �ʱ�ȭ
    mainCharacter = make_unique<Character>
        (glm::vec3(-3.f, 0.f, -3.f));// Position 

    // ó������ �� ž��
    mainCamera->TopView();

    // �� �ʱ�ȭ (���� ��ġ�� ����)
    InitializeEnemies();
}

void Scene::Update(float deltaTime)
{
    // �÷��̾� ���� ������Ʈ
    mainCharacter->Update(deltaTime);
    mainCharacter->UpdateBullets(deltaTime); // �Ѿ� ������Ʈ

    // �� ���� ������Ʈ
    glm::vec3 playerPosition = mainCharacter->GetPosition(); // �÷��̾� ��ġ
    for (auto& enemy : enemies)
    {
        enemy->Update(deltaTime, playerPosition, mazeMap, blockSize);
    }
}

void Scene::Render()
{
    // ī�޶�� Controller���� ����
    

    
    // ���� ����
    mainLight->ApplyLighting(SceneShader, mainCamera->GetPosition());

    // �÷��̾� ������
    mainCharacter->Render(SceneShader);

    // ĳ������ �Ѿ� ������
    const auto& bullets = mainCharacter->GetBullets();
    for (const auto& bullet : bullets) {
        bullet->Render(SceneShader);
    }

    // Actor ������
    for (const auto& actor : actors) {
        actor->Render(SceneShader);
    }


    // �� ������
    for (const auto& enemy : enemies) {
        if (enemy->IsActive()) {
            enemy->Render(SceneShader);
        }
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

void Scene::InitializeEnemies()
{
    std::vector<glm::vec3> emptySpaces;

    // �̷� �����Ϳ��� �� ����(0) ����
    for (int y = 0; y < mazeMap.size(); ++y)
    {
        for (int x = 0; x < mazeMap[y].size(); ++x)
        {
            if (mazeMap[y][x] == 0) // �� ����
            {
                emptySpaces.push_back(glm::vec3(x * blockSize.x, 0.0f, y * blockSize.z));
            }
        }
    }

    // ���� ���� ����
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, emptySpaces.size() - 1);

    // �� 10���� ���� ��ġ�� ��ġ
    for (int i = 0; i < 10; ++i)
    {
        int randomIndex = dist(gen); // ���� �ε��� ����
        glm::vec3 enemyPosition = emptySpaces[randomIndex];

        // �� ����
        auto enemy = std::make_unique<Enemy>(enemyPosition);

        // ��Ʈ�� ��� ����
        glm::vec3 patrolStart = enemyPosition;
        glm::vec3 patrolEnd = enemyPosition + glm::vec3(10.f, 0.f, 0.f); // 10���� �Ÿ� ��Ʈ��
        enemy->SetPatrolPoints(patrolStart, patrolEnd);

        // �� �߰�
        enemies.push_back(std::move(enemy));

        // ���õ� ��ġ ���� (�ߺ� ����)
        emptySpaces.erase(emptySpaces.begin() + randomIndex);
    }
}
