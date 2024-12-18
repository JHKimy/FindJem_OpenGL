#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Actor.h"
#include "Character.h"

#include "MazeGenerator.h"
#include "Controller.h"

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

    // 처음시작 시 탑뷰
    mainCamera->TopView();

    // 플레이어 캐릭터 초기화
    mainCharacter = make_unique<Character>
        (glm::vec3(-3.f, 0.f, -3.f));// Position                    
}

void Scene::Update(float deltaTime)
{
    // 플레이어 상태 업데이트
    mainCharacter->Update(deltaTime);
}

void Scene::Render()
{
    // 카메라는 Controller에서 관리
    
    // 조명 설정
    mainLight->ApplyLighting(SceneShader, mainCamera->GetPosition());

    // 플레이어 렌더링
    mainCharacter->Render(SceneShader);

    // Actor 렌더링
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
    // 미로의 블럭 1개 크기
    glm::vec3 blockSize(5.f, 3.f, 5.f);

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