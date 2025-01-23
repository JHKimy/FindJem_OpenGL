#pragma once
#include "Bullet.h"
#include "Types.h"
//#include "pch.h"

class Character
{


public:
    Character() = default;       // 기본 생성자 추가
    Character(int id);

    int     playerID;       // 플레이어 ID

    bool    bActive = true;
    bool    GetActive()
    {
        return bActive;
    }








    //// 위치
    //struct Position
    //{
    //    float x, y, z;
    //}position;

    vec3 position;

    void SetPosition(const vec3& newPosition) {
        position = newPosition;
    }

    vec3& GetPosition(){
        return position;
    }

    // 크기
    struct Rotation
    {
        float pitch, yaw, roll;
    }rotation;

    struct moveDir
    {
        float x, y, z;
    }moveDir;


    // float   scale;          // 크기

    // 전방 벡터
    struct ForwardVector
    {
        float x, y, z;
    }forwardVector;


    float   yaw;

    float   boundingRadius; // 충돌 구체 반지름

    //int     health;
    float   mass;
    float   moveSpeed;
    bool    isJumping;
    bool    isReady;
    float   jumpSpeed;
    float   gravity;


    int defeatedEnemies = 0; // 적 제거 카운트

    void IncrementDefeatedEnemies() 
    {
        if (isClear) return; // 이미 패배 상태면 처리하지 않음

        defeatedEnemies++;
        g_totalDefeatedEnemies++; // 글로벌 변수 업데이트

        //if (defeatedEnemies == g_enemies.size()) {
        if (g_totalDefeatedEnemies == 5) {
            printf("Character win\n");
        }

    }
    // 총알
    BulletPool bulletPool;



    void Move(char key);
    void Rotate(float deltaYaw);
    void Jump();
    void Update(float deltaTime);
    void Shoot();


    //Position& GetPosition() {
    //    return position;
    //}


    //void TakeDamage(int amount);
    //bool IsGameOver() const;
    //float GetMoveSpeed();
    //void SetMoveSpeed(float speed);


    //BulletPool& GetBulletPool();

    //int GetDefeatedEnemies() const; // 적 제거 카운트 반환
    //void IncrementDefeatedEnemies(); // 적 제거 카운트 증가


    float GetPostionX();
    float GetPostionY();
    float GetPostionZ();



    float GetForwordVecX();
    float GetForwordVecZ();
    float GetYaw();


    int health = 50;                // 초기 체력 100
    bool isDefeated = false;         // 체력이 0이 되면 true
    bool isClear = false;        

    float lastDamageTime = 0.0f;     // 마지막으로 체력 감소된 시간



    void TakeDamage(int damage) {

        if (isDefeated) return; // 이미 패배 상태면 처리하지 않음

        health -= damage;
        if (health <= 0) {
            health = 0;
            bActive = false; // 캐릭터 비활성화
            printf("Character defeated\n");
        }
    }

    bool CanTakeDamage(float currentTime) {
        // 3초마다 체력 감소 가능 여부 확인
        return (currentTime - lastDamageTime) >= 0.5f;
    }

    void SetLastDamageTime(float currentTime) {
        lastDamageTime = currentTime; // 마지막 체력 감소 시간 갱신
    }

    bool IsDefeated() const {
        return isDefeated;
    }

    int  GetHealth() {
        return health;
    }
};