#include "Character.h"
#include <cmath>

void Character::Move(float dirX, float dirY, float dirZ)
{
    // 방향 벡터 계산
    float magnitude = std::sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
    float normX = dirX / magnitude;
    float normY = dirY / magnitude;
    float normZ = dirZ / magnitude;

    // 위치 업데이트
    position.x += normX * moveSpeed;
    position.y += normY * moveSpeed;
    position.z += normZ * moveSpeed;
}

void Character::Rotate(float deltaYaw)
{
    // Yaw 값 업데이트
    yaw += deltaYaw;
    if (yaw > 360.0f) yaw -= 360.0f;
    if (yaw < 0.0f) yaw += 360.0f;

    // 전방 벡터 업데이트 (Pitch는 항상 0)
    float radianYaw = yaw * (3.14159265359f / 180.0f); // 도(degree)를 라디안(radian)으로 변환
    forwardVector.x = std::cos(radianYaw);
    forwardVector.y = 0.0f; // 캐릭터는 수평 방향만 회전
    forwardVector.z = std::sin(radianYaw);

    // 정규화
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
        jumpSpeed = 10.f; // 초기 점프 속도
    }
}

void Character::Update(float deltaTime)
{
    // 중력 적용
    if (isJumping)
    {
        position.y += jumpSpeed * deltaTime;
        jumpSpeed -= gravity * mass * deltaTime;

        // 땅에 내려올때
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
