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

// 씬 생성
Scene::Scene(GLuint shaderProgram) 
    :   SceneShader(shaderProgram)
{
    // 카메라 생성
    mainCamera = make_unique<Camera>(shaderProgram);
    // 라이트 생성 (이 게임에선 하나만 필요)
    mainLight = make_unique<Light>(
        glm::vec3(0.0f, 10.0f, 10.0f),  // pos
        glm::vec3(1.0f));               // color

    // 미로 생성기
    mazeGenerator = make_unique<MazeGenerator>(15, 15);

    // 미로 큐브 1개당 크기
    blockSize = glm::vec3(5.f, 3.f, 5.f);
}

// 씬 초기화
void Scene::Initialize()
{
    // MazeGenerator를 통해 미로 생성
    mazeGenerator->GeneratePrimMaze();
    // 입구 뚫기
    mazeGenerator->addEntrances();
    // 미로 데이터 (0과 1)
    mazeMap = mazeGenerator->GetMaze();



    // 렌더링 해야할 actors에 큐브형태로 추가
    InitializeMaze();



    // 플레이어 캐릭터 초기화
    mainCharacter = make_unique<Character>
        (glm::vec3(-3.f, 0.f, -3.f));// Position 

    // 처음시작 시 탑뷰
    mainCamera->TopView();

    // 적 초기화 (랜덤 위치에 생성)
    InitializeEnemies();
}

void Scene::Update(float deltaTime)
{
    // 플레이어 상태 업데이트
    mainCharacter->Update(deltaTime);
    mainCharacter->UpdateBullets(deltaTime); // 총알 업데이트

    // 적 상태 업데이트
    glm::vec3 playerPosition = mainCharacter->GetPosition(); // 플레이어 위치
    for (auto& enemy : enemies)
    {
        enemy->Update(deltaTime, playerPosition, mazeMap, blockSize);
    }
}

void Scene::Render()
{
    // 카메라는 Controller에서 관리
    

    
    // 조명 설정
    mainLight->ApplyLighting(SceneShader, mainCamera->GetPosition());

    // 플레이어 렌더링
    mainCharacter->Render(SceneShader);

    // 캐릭터의 총알 렌더링
    const auto& bullets = mainCharacter->GetBullets();
    for (const auto& bullet : bullets) {
        bullet->Render(SceneShader);
    }

    // Actor 렌더링
    for (const auto& actor : actors) {
        actor->Render(SceneShader);
    }


    // 적 렌더링
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
                    glm::vec3(blockSize),   // 크기
                    glm::vec3(0),           // 회전값
                    glm::vec3(0, 0, 1)));   // 색상
            }
        }
    }
}

void Scene::InitializeEnemies()
{
    std::vector<glm::vec3> emptySpaces;

    // 미로 데이터에서 빈 공간(0) 추출
    for (int y = 0; y < mazeMap.size(); ++y)
    {
        for (int x = 0; x < mazeMap[y].size(); ++x)
        {
            if (mazeMap[y][x] == 0) // 빈 공간
            {
                emptySpaces.push_back(glm::vec3(x * blockSize.x, 0.0f, y * blockSize.z));
            }
        }
    }

    // 랜덤 엔진 설정
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, emptySpaces.size() - 1);

    // 적 10개를 랜덤 위치에 배치
    for (int i = 0; i < 10; ++i)
    {
        int randomIndex = dist(gen); // 랜덤 인덱스 선택
        glm::vec3 enemyPosition = emptySpaces[randomIndex];

        // 적 생성
        auto enemy = std::make_unique<Enemy>(enemyPosition);

        // 패트롤 경로 설정
        glm::vec3 patrolStart = enemyPosition;
        glm::vec3 patrolEnd = enemyPosition + glm::vec3(10.f, 0.f, 0.f); // 10단위 거리 패트롤
        enemy->SetPatrolPoints(patrolStart, patrolEnd);

        // 적 추가
        enemies.push_back(std::move(enemy));

        // 선택된 위치 제거 (중복 방지)
        emptySpaces.erase(emptySpaces.begin() + randomIndex);
    }
}
