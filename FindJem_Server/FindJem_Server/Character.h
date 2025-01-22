#pragma once
#include "Bullet.h"
#include "Types.h"

class Character
{


public:
    Character() = default;       // 기본 생성자 추가
    Character(int id);

    int     playerID;       // 플레이어 ID

    // 위치
    struct Position
    {
        float x, y, z;
    }position;

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

    int     health;
    float   mass;
    float   moveSpeed;
    bool    isJumping;
    bool    isReady;
    float   jumpSpeed;
    float   gravity;

    int defeatedEnemies = 0; // 적 제거 카운트

    void IncrementDefeatedEnemies() {
        defeatedEnemies++;
    }
    // 총알
    BulletPool bulletPool;



    void Move(char key);
    void Rotate(float deltaYaw);
    void Jump();
    void Update(float deltaTime);
    void Shoot();





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

};