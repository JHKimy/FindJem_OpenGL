#pragma once
#include "Bullet.h"
#include "Types.h"
//#include "pch.h"

class Character
{


public:
    Character() = default;       // �⺻ ������ �߰�
    Character(int id);

    int     playerID;       // �÷��̾� ID

    bool    bActive = true;
    bool    GetActive()
    {
        return bActive;
    }








    //// ��ġ
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

    //int     health;
    float   mass;
    float   moveSpeed;
    bool    isJumping;
    bool    isReady;
    float   jumpSpeed;
    float   gravity;


    int defeatedEnemies = 0; // �� ���� ī��Ʈ

    void IncrementDefeatedEnemies() 
    {
        if (isClear) return; // �̹� �й� ���¸� ó������ ����

        defeatedEnemies++;
        g_totalDefeatedEnemies++; // �۷ι� ���� ������Ʈ

        //if (defeatedEnemies == g_enemies.size()) {
        if (g_totalDefeatedEnemies == 5) {
            printf("Character win\n");
        }

    }
    // �Ѿ�
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

    //int GetDefeatedEnemies() const; // �� ���� ī��Ʈ ��ȯ
    //void IncrementDefeatedEnemies(); // �� ���� ī��Ʈ ����


    float GetPostionX();
    float GetPostionY();
    float GetPostionZ();



    float GetForwordVecX();
    float GetForwordVecZ();
    float GetYaw();


    int health = 50;                // �ʱ� ü�� 100
    bool isDefeated = false;         // ü���� 0�� �Ǹ� true
    bool isClear = false;        

    float lastDamageTime = 0.0f;     // ���������� ü�� ���ҵ� �ð�



    void TakeDamage(int damage) {

        if (isDefeated) return; // �̹� �й� ���¸� ó������ ����

        health -= damage;
        if (health <= 0) {
            health = 0;
            bActive = false; // ĳ���� ��Ȱ��ȭ
            printf("Character defeated\n");
        }
    }

    bool CanTakeDamage(float currentTime) {
        // 3�ʸ��� ü�� ���� ���� ���� Ȯ��
        return (currentTime - lastDamageTime) >= 0.5f;
    }

    void SetLastDamageTime(float currentTime) {
        lastDamageTime = currentTime; // ������ ü�� ���� �ð� ����
    }

    bool IsDefeated() const {
        return isDefeated;
    }

    int  GetHealth() {
        return health;
    }
};