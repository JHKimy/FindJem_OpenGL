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
    blockSize(5.f, 3.f, 5.f) // 블록 크기 초기화
{
    mainCamera = make_unique<Camera>(shaderProgram);
    mainLight = make_unique<Light>(
        glm::vec3(0.0f, 10.0f, 10.0f),  // 조명 위치
        glm::vec3(1.0f));               // 조명 색상

    // 사이즈 정해주기**
    mazeMap.resize(15, std::vector<int>(15));

    // 적 개수 초기화
    enemies.resize(4);


    // mazeGenerator = make_unique<MazeGenerator>(15, 15);
}

void Scene::Initialize()
{
    //// 맵 생성 및 변환
    //mazeGenerator->GeneratePrimMaze();
    //mazeGenerator->addEntrances();
    //mazeMap = mazeGenerator->GetMaze();


    InitializeMaze();

    // 캐릭터 초기화
    mainCharacter = make_unique<Character>(glm::vec3(startPos.x, startPos.y, startPos.z));
                                                                                                                                                                                                                                                                                                          

    mainCamera->TopView();

    for (int i{}; i < mainCharacter->bullets.size(); ++i)
    {
        mainCharacter->bullets[i] = make_shared<Bullet>();
    }

    // 적 초기화
    // InitializeEnemies();
}

void Scene::Update(float deltaTime)
{
    // 플레이어 업데이트
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
     
    // 적 상태 관리 및 충돌 처리
    //for (auto& enemy : enemies)
    //{
        //if (!enemy->IsActive()) continue;

        // 이전 위치 저장
        //glm::vec3 prevPosition = enemy->GetPosition() - enemy->GetDirection() * 0.1f;



        // 적 업데이트
        //##### enemy->Update(deltaTime, playerPosition, mazeMap, blockSize);



        //// 적과 벽돌 충돌 감지
        //if (enemy->CheckCollisionWithActors(actors, blockSize - 0.4f))
        //{
        //    enemy->SetPosition(prevPosition); // 충돌 시 이전 위치로 복구
        //}
   // }

    // 적과 총알 충돌 처리
    // HandleBulletEnemyCollisions();

    // 비활성화된 적 제거
    // RemoveInactiveEnemies();

    // 적 제거 카운트 출력
    // std::cout << "Defeated Enemies: " << mainCharacter->GetDefeatedEnemies() << std::endl;
}

void Scene::Render()
{
    mainLight->ApplyLighting(SceneShader, mainCamera->GetPosition());
    mainCharacter->Render(SceneShader);

    if (g_isOtherCharacter) {
        otherCharacter1->Render(SceneShader);
    }


    //// 총알 렌더링
    //const auto& bullets = mainCharacter->GetBulletPool().GetAllBullets();
    //for (const auto& bullet : bullets)
    //{
    //    if (bullet->IsActive()) bullet->Render(SceneShader);
    //}

    // 총알 렌더링
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

    // 액터 렌더링
    for (const auto& actor : actors)
    {
        actor->Render(SceneShader);
    }

    // 적 렌더링
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

    //// 빈 공간 찾기
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

    //// 랜덤으로 적 생성
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
                enemy->TakeDamage(1, bulletDirection); // 적 체력 감소
                bullet->DeActivate();                 // 총알 비활성화

                printf("Enemy hit\n");

                if (!enemy->IsActive())
                {
                    printf("Enemy defeated\n");
                    mainCharacter->IncrementDefeatedEnemies(); // 적 제거 카운트 증가
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
