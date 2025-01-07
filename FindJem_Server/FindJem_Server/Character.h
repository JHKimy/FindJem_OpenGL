#pragma once


class Character
{
public:
    int     playerID;       // 플레이어 ID

    // 위치
    struct Position
    {
        float x, y, z;
    }positon;

    // 크기
    struct Rotation
    {
        float pitch, yaw, roll;
    }rotation;

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
    float   jumpSpeed;
    float   gravity;

    int defeatedEnemies = 0; // 적 제거 카운트
    
    //BulletPool bulletPool;


    void Move(float dirX, float dirY, float dirZ);
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

};