#pragma once
#include "Bullet.h"
#include "Types.h"

class Character
{


public:
    Character() = default;       // �⺻ ������ �߰�
    Character(int id);

    int     playerID;       // �÷��̾� ID

    // ��ġ
    struct Position
    {
        float x, y, z;
    }position;

    // ũ��
    struct Rotation
    {
        float pitch, yaw, roll;
    }rotation;

    struct moveDir
    {
        float x, y, z;
    }moveDir;


    // float   scale;          // ũ��

    // ���� ����
    struct ForwardVector
    {
        float x, y, z;
    }forwardVector;


    float   yaw;

    float   boundingRadius; // �浹 ��ü ������

    int     health;
    float   mass;
    float   moveSpeed;
    bool    isJumping;
    bool    isReady;
    float   jumpSpeed;
    float   gravity;

    int defeatedEnemies = 0; // �� ���� ī��Ʈ

    void IncrementDefeatedEnemies() {
        defeatedEnemies++;
    }
    // �Ѿ�
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

    //int GetDefeatedEnemies() const; // �� ���� ī��Ʈ ��ȯ
    //void IncrementDefeatedEnemies(); // �� ���� ī��Ʈ ����


    float GetPostionX();
    float GetPostionY();
    float GetPostionZ();
    float GetForwordVecX();
    float GetForwordVecZ();
    float GetYaw();

};