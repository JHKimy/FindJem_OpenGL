#include "Character.h"
#include <cmath>

void Character::Move(float dirX, float dirY, float dirZ)
{
    // ���� ���� ���
    float magnitude = std::sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    float normX = dirX / magnitude;
    float normY = dirY / magnitude;
    float normZ = dirZ / magnitude;

    // ��ġ ������Ʈ
    position.x += normX * moveSpeed;
    position.y += normY * moveSpeed;
    position.z += normZ * moveSpeed;
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
    return positon.x;
}

float Character::GetPostionY()
{
    return positon.y;
}

float Character::GetPostionZ()
{
    return position.z;
}
