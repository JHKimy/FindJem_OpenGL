#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Actor.h"
#include "Character.h"
#include "Bullet.h"
#include "Enemy.h"
//#include "MazeGenerator.h"
#include "Controller.h"
#include "Global.h"
#include <random>
#include <iostream>

Scene::Scene(GLuint shaderProgram)
    : SceneShader(shaderProgram),
    blockSize(5.f, 3.f, 5.f) // ��� ũ�� �ʱ�ȭ
{
    mainCamera = make_unique<Camera>(shaderProgram);
    mainLight = make_unique<Light>(
        glm::vec3(0.0f, 10.0f, 10.0f),  // ���� ��ġ
        glm::vec3(1.0f));               // ���� ����

    // ������ �����ֱ�**
    mazeMap.resize(15, std::vector<int>(15));

    // �� ���� �ʱ�ȭ
    enemies.resize(4);


    // mazeGenerator = make_unique<MazeGenerator>(15, 15);
}

void Scene::Initialize()
{
    //// �� ���� �� ��ȯ
    //mazeGenerator->GeneratePrimMaze();
    //mazeGenerator->addEntrances();
    //mazeMap = mazeGenerator->GetMaze();


    InitializeMaze();

    // ĳ���� �ʱ�ȭ
    mainCharacter = make_unique<Character>(glm::vec3(startPos.x, startPos.y, startPos.z));
                                                                                                                                                                                                                                                                                                          

    mainCamera->TopView();

    for (int i{}; i < mainCharacter->bullets.size(); ++i)
    {
        mainCharacter->bullets[i] = make_shared<Bullet>();
    }

    // �� �ʱ�ȭ
    // InitializeEnemies();
}

void Scene::Update(float deltaTime)
{
    // �÷��̾� ������Ʈ
    mainCharacter->Update(deltaTime);
    // otherCharacter1->Update(deltaTime);

    if (g_bReady) {
        otherCharacter1 = make_unique<Character>(glm::vec3(position.x, position.y, position.z));

        otherCharacter1->SetForwardVector(glm::vec3(0.5f, 0.f, 0.5f));
        g_isOtherCharacter = true;
        g_bReady = false;
    }


    glm::vec3 playerPosition = mainCharacter->GetPosition();

    // cout << enemies[0].get()->GetPosition().z<<endl;
     
    // �� ���� ���� �� �浹 ó��
    //for (auto& enemy : enemies)
    //{
        //if (!enemy->IsActive()) continue;

        // ���� ��ġ ����
        //glm::vec3 prevPosition = enemy->GetPosition() - enemy->GetDirection() * 0.1f;



        // �� ������Ʈ
        //##### enemy->Update(deltaTime, playerPosition, mazeMap, blockSize);



        //// ���� ���� �浹 ����
        //if (enemy->CheckCollisionWithActors(actors, blockSize - 0.4f))
        //{
        //    enemy->SetPosition(prevPosition); // �浹 �� ���� ��ġ�� ����
        //}
   // }

    // ���� �Ѿ� �浹 ó��
    // HandleBulletEnemyCollisions();

    // ��Ȱ��ȭ�� �� ����
    // RemoveInactiveEnemies();

    // �� ���� ī��Ʈ ���
    // std::cout << "Defeated Enemies: " << mainCharacter->GetDefeatedEnemies() << std::endl;
}

void Scene::Render()
{
    mainLight->ApplyLighting(SceneShader, mainCamera->GetPosition());
    mainCharacter->Render(SceneShader);

    if (g_isOtherCharacter) {
        otherCharacter1->Render(SceneShader);
    }


    //// �Ѿ� ������
    //const auto& bullets = mainCharacter->GetBulletPool().GetAllBullets();
    //for (const auto& bullet : bullets)
    //{
    //    if (bullet->IsActive()) bullet->Render(SceneShader);
    //}

    // �Ѿ� ������
    const auto& bullets = mainCharacter->bullets;
    //for (const auto& bullet : bullets)
    for (int i{};i<bullets.size();++i)
    {
        if (bullets[i]->IsActive()) 
        {
           // cout << "bulletNum : " << i << " Render " << endl;
            bullets[i]->Render(SceneShader);
        }
    }

    // ���� ������
    for (const auto& actor : actors)
    {
        actor->Render(SceneShader);
    }

    // �� ������
    for (const auto& enemy : enemies)
    {
        if (enemy->IsActive())
        {
            enemy->Render(SceneShader);
            //cout << "REndfs"<<endl;
        }
    }
}

void Scene::SetMaze(int mazeMapData[15][15])
{

    for (int i{}; i < 15; ++i) {
        for (int j{}; j < 15; ++j)
        {
            mazeMap[i][j] = mazeMapData[i][j];
        }
    }
}

void Scene::InitializeMaze()
{
    for (int z = 0; z < mazeMap.size(); ++z)
    {
        for (int x = 0; x < mazeMap[z].size(); ++x)
        {
            if (mazeMap[z][x] == 1)
            {
                actors.push_back(make_unique<Actor>(
                    "Cube.obj",
                    glm::vec3(x * blockSize.x, 0.0f, z * blockSize.z),
                    blockSize,
                    glm::vec3(0),
                    glm::vec3(0, 0, 1)));
            }
        }
    }
}

void Scene::InitializeEnemies()
{
    //std::vector<glm::vec3> emptySpaces;

    //// �� ���� ã��
    //for (int z = 0; z < mazeMap.size(); ++z)
    //{
    //    for (int x = 0; x < mazeMap[z].size(); ++x)
    //    {
    //        if (mazeMap[z][x] == 0)
    //        {
    //            emptySpaces.push_back(glm::vec3(x * blockSize.x, 0.0f, z * blockSize.z));
    //        }
    //    }
    //}

    //// �������� �� ����
    //std::random_device rd;
    //std::mt19937 gen(rd());
    //std::uniform_int_distribution<> dist(0, emptySpaces.size() - 1);

    //for (int i = 0; i < 1; ++i)
    //{
    //    int randomIndex = dist(gen);
    //    glm::vec3 enemyPosition = emptySpaces[randomIndex];
    //    enemies.push_back(std::make_shared<Enemy>(enemyPosition));
    //    emptySpaces.erase(emptySpaces.begin() + randomIndex);
    //}
}

void Scene::HandleBulletEnemyCollisions()
{
    for (auto& enemy : enemies)
    {
        if (!enemy->IsActive()) continue;

        for (const auto& bullet : mainCharacter->GetBulletPool().GetAllBullets())
        {
            if (!bullet->IsActive()) continue;

            if (bullet->CheckCollision(static_cast<Actor*>(enemy.get())))
            {
                glm::vec3 bulletDirection = glm::normalize(bullet->GetDirection());
                enemy->TakeDamage(1, bulletDirection); // �� ü�� ����
                bullet->DeActivate();                 // �Ѿ� ��Ȱ��ȭ

                printf("Enemy hit\n");

                if (!enemy->IsActive())
                {
                    printf("Enemy defeated\n");
                    mainCharacter->IncrementDefeatedEnemies(); // �� ���� ī��Ʈ ����
                    break;
                }
            }
        }
    }
}

void Scene::RemoveInactiveEnemies()
{
    //enemies.erase(
    //    std::remove_if(enemies.begin(), enemies.end(),
    //        [](const std::unique_ptr<Enemy>& enemy) { return !enemy->IsActive(); }),
    //    enemies.end());
}
