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


    health = 100;        // 체력
    moveSpeed = 0.5f;    // 스피드
    isJumping = false;   // 점프 상태
    isReady = false;
    boundingRadius = 1.5f;  // 충돌 범위
    forwardVector.x = 0.f;
    forwardVector.y = 0.f;
    forwardVector.z = 0.f;
    moveDir.x = 0.f;
    moveDir.y = 0.f;
    moveDir.z = 0.f;
    // mass = 2.f;          // 질량
    // gravity(9.8f),       // 중력 영향
    //    bulletPool(30)
    
}

void Character::Move(char key)
{
    if (key == 0/* W */)
    {
        moveDir.x += forwardVector.x;
        //moveDir.y += forwardVector.y;
        moveDir.z += forwardVector.z;
        
    }
    if (key == 1/* S */)
    {
        moveDir.x -= forwardVector.x;
        //moveDir.y -= forwardVector.y;
        moveDir.z -= forwardVector.z;

    }
    if (key == 2/* A */)
    {
        // 외적 계산 (Y축 기준으로 왼쪽 벡터)
        float crossX = - forwardVector.z; // a_y * b_z - a_z * b_y
        float crossY = 0; // a_z * b_x - a_x * b_z
        float crossZ = forwardVector.x; // a_x * b_y - a_y * b_x

        moveDir.x -= crossX;
        // moveDir.y -= crossY;
        moveDir.z -= crossZ;
    }

    if (key == 3/* D */)
    {
        // 외적 계산 (Y축 기준으로 오른쪽 벡터)
        float crossX = -forwardVector.z; // a_y * b_z - a_z * b_y
        float crossY = 0.f; // a_z * b_x - a_x * b_z
        float crossZ = forwardVector.x; // a_x * b_y - a_y * b_x

        moveDir.x += crossX;
        // moveDir.y += crossY;
        moveDir.z += crossZ;
    }

    // === moveDir 정규화 ===
    float length = sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y + moveDir.z * moveDir.z);
    if (length > 0.0f) // 길이가 0이 아닐 때만 정규화
    {
        moveDir.x /= length;
        moveDir.y /= length;
        moveDir.z /= length;
    }

    position.x += moveDir.x * moveSpeed;
    position.y += moveDir.y * moveSpeed;
    position.z += moveDir.z * moveSpeed;


    // moveDir 초기화 (옵션, 다음 이동 계산에서 누적 방지)
    moveDir.x = 0.0f;
    moveDir.y = 0.0f;
    moveDir.z = 0.0f;
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

float Character::GetForwordVecX()
{
    return forwardVector.x;
}

float Character::GetForwordVecZ()
{
    return forwardVector.z;
}

float Character::GetYaw()
{
    return yaw;
}
