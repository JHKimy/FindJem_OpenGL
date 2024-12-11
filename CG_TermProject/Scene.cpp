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
    shaderProgram(shaderProgram) 


{
    mazeMap =
    { {1,1,1,1,1,1},
      {0,0,0,0,0,0},
      {1,0,1,0,1,0},
      {0,1,0,1,0,1}
    };
}

void Scene::Initialize()
{
    InitializeMaze();
    // Actor 초기화
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

    // 플레이어 상태 업데이트
    mainCharacter->Update(deltaTime);


}

void Scene::Render()
{
    //glm::vec3 cameraOffset(0.0f, 5.0f, 10.0f); // 카메라가 캐릭터 뒤에 위치
    //glm::vec3 characterPosition = mainCharacter->GetPosition();
    //glm::vec3 characterDirection = mainCharacter->GetDirection();

    if (Command[Num1]) {
        // mainCamera.SwitchToFirstPerson(
        // mainCharacter->GetPosition(), mainCharacter->GetDirection());
        //mainCamera.UpdatePosition(characterPosition + cameraOffset);
        //mainCamera.UpdateTarget(characterPosition + mainCharacter->GetDirection());
        mainCamera.FirstPersonView(
            GetCharacter()->GetPosition()
            );

    }

    if (Command[Num2]) {
        mainCamera.TopView();
    }

    // 카메라 설정
    mainCamera.ApplyCamera(shaderProgram);



    // 조명 설정
    mainLight.ApplyLighting(shaderProgram, mainCamera.GetPosition());

    // 플레이어 렌더링
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

void Scene::InitializeMaze()
{
    glm::vec3 blockSize(1.f, 1.f, 1.f);

    for (int y{}; y < mazeMap.size(); ++y) {
        for (int x{}; x < mazeMap[y].size(); ++x) {
            if (mazeMap[y][x] == 1) {
                actors.push_back(new Actor(
                        "Cube.obj",             // filePath
                        glm::vec3(x * blockSize.x, 0.0f, y * blockSize.z), // Position
                        glm::vec3(blockSize),           // Scale
                        glm::vec3(0),           // Rotation
                        glm::vec3(0, 0, 1)));   // Color
            }
        }
    }

}
