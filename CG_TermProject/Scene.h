#pragma once

#include <vector>
#include <memory> // 스마트 포인터 사용
#include "MazeGenerator.h" // Scene.h에 직접 포함 (덜 권장됨)
#include <GL/glew.h> // OpenGL의 GLuint 및 관련 타입 정의
#include "Enemy.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

// 전방 선언
class Camera;
class Light;
class Actor;
class Character;
class Enemy;
class MazeGenerator;

class Scene
{
private:
    // 셰이더 프로그램 ID
    GLuint SceneShader;

    // 카메라
    unique_ptr<Camera> mainCamera;
    // 조명 
    unique_ptr<Light> mainLight;
    // 씬에 포함된 Actor 객체들
    vector<unique_ptr<Actor>> actors;
    // 플레이어 캐릭터
    unique_ptr<Character> mainCharacter;
    // 미로 생성기
    unique_ptr<MazeGenerator> mazeGenerator;

    vector<unique_ptr<Enemy>> enemies; // 적 객체들

    // 미로 데이터
    vector<vector<int>> mazeMap;
    // 미로데이터를 Actor로 변환
    void InitializeMaze();

    glm::vec3 blockSize; // 블록 크기


    // 적 소환
    void InitializeEnemies();

public:
    // 생성자 및 소멸자
    Scene(GLuint shaderProgram);
    // 유니크 포인터가 자동으로 메모리 정리
    ~Scene() = default;

    void Initialize();
    void Update(float deltaTime);
    void Render();

    Character* GetCharacter();
    Camera* GetCamera();
    const std::vector<std::unique_ptr<Actor>>& GetActors() const;
};