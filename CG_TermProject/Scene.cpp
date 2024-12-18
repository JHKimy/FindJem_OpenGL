#include "Scene.h"
#include "Controller.h"
#include "MazeGenerator.h"


// 씬 생성
Scene::Scene(GLuint shaderProgram) :

   mainCamera(shaderProgram),
    
    mainLight(
        glm::vec3(0.0f, 10.0f, 10.0f),  // pos
        glm::vec3(1.0f)),               // color

    shaderProgram(shaderProgram) 
{
    // MazeGenerator 초기화
    mazeGenerator = new MazeGenerator(15, 15); // 15x15 크기의 미로 생성
}





// 씬 초기화
void Scene::Initialize()
{
    // MazeGenerator를 통해 미로 생성
    mazeGenerator->GeneratePrimMaze();
    mazeGenerator->addEntrances();
    mazeMap = mazeGenerator->GetMaze();

    InitializeMaze();




    mainCamera.TopView();






    // Actor 초기화
    //actors.push_back(new Actor(
    //    "Cube.obj",             // filePath
    //    glm::vec3(1,1,1),       // Position
    //    glm::vec3(1),           // Scale
    //    glm::vec3(0),           // Rotation
    //    glm::vec3(1, 0, 0)));   // Color 
    
    mainCharacter = new Character(
        "Boss.obj",              // filePath
        glm::vec3(-3.f, 0.f, -3.f),// Position,
        glm::vec3(.05f),          // Scale
        glm::vec3(0),            // Rotation
        glm::vec3(0, 1, 0),      // Color 
        0.5f,                   // speed
        100);                    // health

}

void Scene::Update(float deltaTime)
{

    // 플레이어 상태 업데이트
    mainCharacter->Update(deltaTime);


}

void Scene::Render()
{

    //if (Command[Num1]) {
    //    mainCamera.FirstPersonView(GetCharacter()->GetPosition());
    //}
    //if (Command[Num2]) {
    //    mainCamera.TopView();
    //}
    // 카메라 설정
    //mainCamera.ApplyCamera(shaderProgram);



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
    for (Actor* actor : actors)
    {
        delete actor;
    }
    actors.clear();

    delete mazeGenerator;
    delete mainCharacter;
}

Character* Scene::GetCharacter()
{
    return mainCharacter;
}

Camera* Scene::GetCamera()
{
    return &mainCamera;
}

const std::vector<Actor*>& Scene::GetActors() const
{
    return actors;
}

void Scene::InitializeMaze()
{
    // 미로의 블럭 1개 크기
    glm::vec3 blockSize(5.f, 2.f, 5.f);

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