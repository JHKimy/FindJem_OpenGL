#pragma once


class Character
{
public:
    int     playerID;       // �÷��̾� ID

    // ��ġ
    struct Position
    {
        float x, y, z;
    }positon;

    // ũ��
    struct Rotation
    {
        float pitch, yaw, roll;
    }rotation;

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
    float   jumpSpeed;
    float   gravity;

    int defeatedEnemies = 0; // �� ���� ī��Ʈ
    
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

    //int GetDefeatedEnemies() const; // �� ���� ī��Ʈ ��ȯ
    //void IncrementDefeatedEnemies(); // �� ���� ī��Ʈ ����


    float GetPostionX();
    float GetPostionY();
    float GetPostionZ();

};