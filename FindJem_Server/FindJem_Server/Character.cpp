#define NOMINMAX // Windows.h와의 충돌 방지
#include "Character.h"
#include <cmath>
#include <algorithm> // std::max, std::min
#include "pch.h"

Character::Character(int id) : bulletPool(30)
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
    boundingRadius = 1.f;  // 충돌 범위
    forwardVector.x = 0.f;
    forwardVector.y = 0.f;
    forwardVector.z = 0.f;
    moveDir.x = 0.f;
    moveDir.y = 0.f;
    moveDir.z = 0.f;
    // mass = 2.f;          // 질량
    // gravity(9.8f),       // 중력 영향
    //bulletPool(30)

}

void Character::Move(char key)
{
    // g_mazeMap의 출력 상태를 추적하는 static 변수
    static bool hasPrinted = false;

    // 처음 한 번만 g_mazeMap 출력
    if (!hasPrinted)
    {
        const float blockDiameter = 5.0f; // 블록의 지름
        //std::cout << "Current Maze Map:" << std::endl;

        for (int z = 0; z < g_mazeMap.size(); ++z)
        {
            for (int x = 0; x < g_mazeMap[z].size(); ++x)
            {
                if (g_mazeMap[z][x] == 1) // 벽인 경우
                {
                    // 화면상의 좌표 (월드 좌표)
                    float blockCenterX = x * blockDiameter;
                    float blockCenterZ = z * blockDiameter;
                   /* std::cout << "Wall at Maze Coordinates (" << z << ", " << x << ") -> "
                        << "World Coordinates (" << blockCenterX << ", " << blockCenterZ << ")" << std::endl;*/
                }
            }
        }
        std::cout << "-------------------------" << std::endl;

        // 출력 상태 업데이트
        hasPrinted = true;
    }
    // 임시 이동 방향 계산
    if (key == 0 /* W */)
    {
        moveDir.x += forwardVector.x;
        moveDir.z += forwardVector.z;
    }
    else if (key == 1 /* S */)
    {
        moveDir.x -= forwardVector.x;
        moveDir.z -= forwardVector.z;
    }
    else if (key == 2 /* A */)
    {
        float crossX = -forwardVector.z;
        float crossZ = forwardVector.x;

        moveDir.x -= crossX;
        moveDir.z -= crossZ;
    }
    else if (key == 3 /* D */)
    {
        float crossX = -forwardVector.z;
        float crossZ = forwardVector.x;

        moveDir.x += crossX;
        moveDir.z += crossZ;
    }

    // === moveDir 정규화 ===
    float length = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y + moveDir.z * moveDir.z);
    if (length > 0.0f)
    {
        moveDir.x /= length;
        moveDir.z /= length;
    }

    // 임시 위치 계산
    float newX = position.x + moveDir.x * moveSpeed;
    float newZ = position.z + moveDir.z * moveSpeed;

    // 충돌 검사: 모든 블록에 대해 각각 검사
    const int mazeRows = g_mazeMap.size();
    const int mazeCols = g_mazeMap[0].size();
    bool collision = false;

    for (int z = 0; z < mazeRows; ++z)
    {
        for (int x = 0; x < mazeCols; ++x)
        {
            if (g_mazeMap[z][x] == 1) // 벽인 경우
            {
                float wallX = x * 5.f; // 벽의 X 좌표
                float wallZ = z * 5.f; // 벽의 Z 좌표

                // === 충돌 조건 계산 ===
                float halfBlockSize = 2.5f; // 블록 한 변의 절반 크기 (3.0 / 2)
                if (std::abs(newX - wallX) < (halfBlockSize + boundingRadius) && // X축 충돌
                    std::abs(newZ - wallZ) < (halfBlockSize + boundingRadius))   // Z축 충돌
                {
                    collision = true; // 충돌 발생
                    std::cout << "Collision detected with wall at (" << x << ", " << z << ")" << std::endl;
                    break; // 충돌이 발생했으므로 더 이상 검사하지 않음
                }
            }
        }
        if (collision)
            break;
    }
    // 충돌이 없을 경우에만 위치 갱신
    if (!collision)
    {
        position.x = newX;
        position.z = newZ;
    }

    // moveDir 초기화
    moveDir.x = 0.0f;
    moveDir.y = 0.0f;
    moveDir.z = 0.0f;

    //cout << "characterPos" << position.x << ", " << position.z << endl;
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
    // 총알의 초기 위치를 설정 (캐릭터의 위치 + forwardVector * 거리)
    vec3 bulletPos = vec3{
        position.x + forwardVector.x * 1.0f, // 총구의 x 좌표
        position.y + forwardVector.y * 1.0f, // 총구의 y 좌표
        position.z + forwardVector.z * 1.0f  // 총구의 z 좌표
    };

    vec3 bulletForward = vec3{
       forwardVector.x, // 총구의 x 좌표
       forwardVector.y, // 총구의 y 좌표
       forwardVector.z// 총구의 z 좌표
    };

    auto bullet = bulletPool.GetBullet(); // 비활성화된 총알 가져오기
    if (bullet) {
        bullet->Activate(bulletPos, bulletForward); // 활성화 및 초기화
    }

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
