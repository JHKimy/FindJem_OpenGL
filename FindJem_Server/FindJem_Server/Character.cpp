#include "Character.h"
#include <cmath>

Character::Character(int id)
{
    if (id == 0) {
        position.x = -10.f;
        position.y = 0.f;
        position.z = 5.f;
    }
    else if (id == 1) {
        position.x = -15.f;
        position.y = 0.f;
        position.z = 5.f;
    }
    else if (id == 2) {
        position.x = -12.f;
        position.y = 0.f;
        position.z = 5.f;
    }


    health = 100;        // ü��
    moveSpeed = 0.5f;    // ���ǵ�
    isJumping = false;   // ���� ����
    isReady = false;
    boundingRadius = 1.5f;  // �浹 ����
    forwardVector.x = 1.f;
    forwardVector.y = 0.f;
    forwardVector.z = 0.f;
    // mass = 2.f;          // ����
    // gravity(9.8f),       // �߷� ����
    //    bulletPool(30)
    
}

void Character::Move(char key)
{
    if (key == 0/* W */)
    {
        moveDir.x += forwardVector.x;
        moveDir.y += forwardVector.y;
        moveDir.z += forwardVector.z;
        
    }
    if (key == 1/* S */)
    {
        moveDir.x -= forwardVector.x;
        moveDir.y -= forwardVector.y;
        moveDir.z -= forwardVector.z;

    }
    if (key == 2/* A */)
    {
        // ���� ��� (Y�� �������� ���� ����)
        float crossX = forwardVector.y * 0.0f - forwardVector.z * 1.0f; // a_y * b_z - a_z * b_y
        float crossY = forwardVector.z * 0.0f - forwardVector.x * 0.0f; // a_z * b_x - a_x * b_z
        float crossZ = forwardVector.x * 1.0f - forwardVector.y * 0.0f; // a_x * b_y - a_y * b_x

        moveDir.x -= crossX;
        moveDir.y -= crossY;
        moveDir.z -= crossZ;
    }

    if (key == 3/* D */)
    {
        // ���� ��� (Y�� �������� ������ ����)
        float crossX = forwardVector.y * 0.0f - forwardVector.z * 1.0f; // a_y * b_z - a_z * b_y
        float crossY = forwardVector.z * 0.0f - forwardVector.x * 0.0f; // a_z * b_x - a_x * b_z
        float crossZ = forwardVector.x * 1.0f - forwardVector.y * 0.0f; // a_x * b_y - a_y * b_x

        moveDir.x += crossX;
        moveDir.y += crossY;
        moveDir.z += crossZ;
    }

    position.x += moveDir.x * moveSpeed;
    position.y += moveDir.y * moveSpeed;
    position.z += moveDir.z * moveSpeed;

}

void Character::Rotate(float deltaYaw)
{
    // Yaw �� ������Ʈ
    yaw += deltaYaw;
    if (yaw > 360.0f) yaw -= 360.0f;
    if (yaw < 0.0f) yaw += 360.0f;

    // ���� ���� ������Ʈ (Pitch�� �׻� 0)
    float radianYaw = yaw * (3.14159265359f / 180.0f); // ��(degree)�� ����(radian)���� ��ȯ
    forwardVector.x = std::cos(radianYaw);
    forwardVector.y = 0.0f; // ĳ���ʹ� ���� ���⸸ ȸ��
    forwardVector.z = std::sin(radianYaw);

    // ����ȭ
    float magnitude = std::sqrt(forwardVector.x * forwardVector.x +
        forwardVector.y * forwardVector.y +
        forwardVector.z * forwardVector.z);
    forwardVector.x /= magnitude;
    forwardVector.y /= magnitude;
    forwardVector.z /= magnitude;
}

void Character::Jump()
{
    if (!isJumping) {
        isJumping = true;
        jumpSpeed = 10.f; // �ʱ� ���� �ӵ�
    }
}

void Character::Update(float deltaTime)
{
    // �߷� ����
    if (isJumping)
    {
        position.y += jumpSpeed * deltaTime;
        jumpSpeed -= gravity * mass * deltaTime;

        // ���� �����ö�
        if (position.y <= 0.0f) {
            position.y = 0.0f;
            isJumping = false;
        }
    }
}

void Character::Shoot()
{
}

float Character::GetPostionX()
{
    return position.x;
}

float Character::GetPostionY()
{
    return position.y;
}

float Character::GetPostionZ()
{
    return position.z;
}
