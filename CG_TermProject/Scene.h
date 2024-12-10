#pragma once

//#include "stdafx.h"
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Actor.h"
#include "Character.h"
#include "Enemy.h"

class Scene
{
private:
    Camera mainCamera;  // 카메라
    Light mainLight;    // 조명
    std::vector<Actor*> actors; // 씬에 포함된 Actor 객체들
    GLuint shaderProgram;   // 셰이더 프로그램 ID
    Character* mainCharacter;  // 플레이어 캐릭터
    //Enemy* enemy;   // 적 객체
    std::vector<Enemy*> enemeys;    // 적 객체들

    //void InitializeCubeBuffers();             // 버퍼 초기화
    //void InitializeEnemies();               // 적 초기화
    //void UpdateEnemies(float deltaTime);    // 적 업데이트
    //void CheckCollisions();                 // 충돌 검사
    //void DrawFloor();                       // 바닥 그리기
    //void DrawCharacter();                   // 캐릭터 그리기
    //void DrawEnemies();                     // 적 그리기
    //void DrawBullets();                     // 총알 그리기


public:
    // 생성자
    Scene(GLuint shaderProgram);
    void Initialize();   // 초기화
    void Update(float deltaTime); // 업데이트
    void Render();       // 렌더링
    void Shutdown();     // 리소스 정리
    Character* GetCharacter();

};