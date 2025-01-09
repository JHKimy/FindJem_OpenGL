#pragma once

#include <vector>
#include <memory> // 스마트 포인터 사용
#include <GL/glew.h> // OpenGL의 GLuint 및 관련 타입 정의
#include "MazeGenerator.h" // 미로 생성기
#include "array"
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

    // ===== 멤버 변수 =====
    GLuint SceneShader; // 셰이더 프로그램 ID

    // 주요 게임 객체
    std::unique_ptr<Camera> mainCamera;         // 메인 카메라
    std::unique_ptr<Light> mainLight;           // 메인 조명
    std::unique_ptr<Character> mainCharacter;   // 플레이어 캐릭터
    
    
    std::unique_ptr<Character> otherCharacter1;   // 플레이어 캐릭터
    std::unique_ptr<Actor> otherCharacter2;   // 플레이어 캐릭터

    
    //std::unique_ptr<MazeGenerator> mazeGenerator; // 미로 생성기

    // 액터와 적 관리
    std::vector<std::unique_ptr<Actor>> actors; // 씬에 포함된 액터들
    
    
    
    
public:
    std::array<std::shared_ptr<Enemy>,1> enemies; // 적들


private:





    std::vector<std::vector<int>> mazeMap; // 미로 데이터

    glm::vec3 blockSize; // 블록 크기


    // ===== 내부 유틸리티 함수 =====
    void InitializeMaze();       // 미로 초기화
    void InitializeEnemies();    // 적 초기화
    void HandleBulletEnemyCollisions(); // 적과 총알 충돌 처리
    void RemoveInactiveEnemies();   // 비활성화 된 적 지우기

public:
    // ===== 생성자 및 소멸자 =====
    explicit Scene(GLuint shaderProgram); // 생성자
    ~Scene() = default; // 소멸자


    // ===== 주요 멤버 함수 =====
    void Initialize();           // 초기화
    void Update(float deltaTime); // 업데이트
    void Render();               // 렌더링


    // ===== 접근자 함수 =====
    Character* GetCharacter() { return mainCharacter.get(); }
    Character* GetOtherCharacter1() { return otherCharacter1.get(); }

    Camera* GetCamera() { return mainCamera.get(); }
    const std::vector<std::unique_ptr<Actor>>& GetActors() const { return actors; };
    const std::vector<std::vector<int>>& GetMazeMap() const { return mazeMap; } 



    // 서버 미로좌표 받아오기
    void SetMaze(int mazeMapData[15][15]);


    //void AddEnemy(float x, float y, float z);
};
