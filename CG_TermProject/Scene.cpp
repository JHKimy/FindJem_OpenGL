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
#include <iostream>

Scene::Scene(GLuint shaderProgram)
    : SceneShader(shaderProgram)
{
    mainCamera = make_unique<Camera>(shaderProgram);
    mainLight = make_unique<Light>(
        glm::vec3(0.0f, 10.0f, 10.0f),  // pos
        glm::vec3(1.0f));               // color

    mazeGenerator = make_unique<MazeGenerator>(15, 15);

    blockSize = glm::vec3(5.f, 3.f, 5.f);

}

void Scene::Initialize()
{
    mazeGenerator->GeneratePrimMaze();
    mazeGenerator->addEntrances();
    mazeMap = mazeGenerator->GetMaze();



    InitializeMaze();


    mainCharacter = make_unique<Character>
        (glm::vec3(-10.f, 0.f, 5.f));// Position 


    mainCamera->TopView();

    InitializeEnemies();
}

void Scene::Update(float deltaTime)
{
    mainCharacter->Update(deltaTime);

    glm::vec3 playerPosition = mainCharacter->GetPosition();
    for (auto& enemy : enemies)
    {
        if (!enemy->IsActive()) continue; // 비활성화된 적은 업데이트하지 않음

        // 기존 위치 저장
        glm::vec3 prevPosition = enemy->GetPosition();

        enemy->Update(deltaTime, playerPosition, mazeMap, blockSize);

        // 적과 벽돌(Actor) 충돌 감지
        if (enemy->CheckCollisionWithActors(actors, blockSize - 0.3f)) {
            enemy->SetPosition(prevPosition); // 충돌 시 이전 위치로 복구
        }
    }

    // 적과 총알 간 충돌 처리
    for (auto& enemy : enemies)
    {
        if (!enemy->IsActive()) continue; // 비활성화된 적은 무시

        for (const auto& bullet : mainCharacter->GetBulletPool().GetAllBullets())
        {
            if (!bullet->IsActive()) continue; // 비활성화된 총알은 무시

            if (bullet->CheckCollision(static_cast<Actor*>(enemy.get())))
            {
                // 총알 방향 계산
                glm::vec3 bulletDirection = glm::normalize(bullet->GetDirection());

                enemy->TakeDamage(1, bulletDirection);  // 체력 감소
              
                bullet->DeActivate(); // 총알 비활성화

                printf("11111111\n"); // 충돌 확인 메시지

                if (!enemy->IsActive())
                {
                    printf("Enemy defeated!\n");
                    break; // 적이 죽으면 다른 총알과의 충돌 검사 중단
                }
            }
        }
    }

    // 비활성화된 적 제거
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) { return !enemy->IsActive(); }),
        enemies.end());




    // BulletPool 상태 출력
    const auto& bulletPool = mainCharacter->GetBulletPool();
    std::cout << "Total Bullets: " << bulletPool.GetAllBullets().size() << std::endl;
    std::cout << "Available Bullets: " << bulletPool.GetAvailableBulletCount() << std::endl;

}

void Scene::Render()
{



    mainLight->ApplyLighting(SceneShader, mainCamera->GetPosition());


    mainCharacter->Render(SceneShader);

    // 총알
    const auto& bullets = mainCharacter->GetBulletPool().GetAllBullets();
    
    for (const auto& bullet : bullets) 
    {
        if (bullet->IsActive()) bullet->Render(SceneShader);
    }


    for (const auto& actor : actors) {
        actor->Render(SceneShader);
    }



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
                    glm::vec3(x * blockSize.x, 0.0f, y * blockSize.z),
                    glm::vec3(blockSize),
                    glm::vec3(0),
                    glm::vec3(0, 0, 1)));
            }
        }
    }
}

void Scene::InitializeEnemies()
{
    std::vector<glm::vec3> emptySpaces;

    for (int y = 0; y < mazeMap.size(); ++y)
    {
        for (int x = 0; x < mazeMap[y].size(); ++x)
        {
            if (mazeMap[y][x] == 0)
            {
                emptySpaces.push_back(glm::vec3(x * blockSize.x, 0.0f, y * blockSize.z));
            }
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, emptySpaces.size() - 1);

    for (int i = 0; i < 10; ++i)
    {
        int randomIndex = dist(gen);
        glm::vec3 enemyPosition = emptySpaces[randomIndex];

        auto enemy = std::make_unique<Enemy>(enemyPosition);

        glm::vec3 patrolStart = enemyPosition;
        glm::vec3 patrolEnd = enemyPosition + glm::vec3(10.f, 0.f, 0.f);
        enemy->SetPatrolPoints(patrolStart, patrolEnd);

        enemies.push_back(std::move(enemy));

        emptySpaces.erase(emptySpaces.begin() + randomIndex);
    }
}