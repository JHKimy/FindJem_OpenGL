#define NOMINMAX // Windows.h���� �浹 ����
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


    health = 100;        // ü��
    moveSpeed = 0.5f;    // ���ǵ�
    isJumping = false;   // ���� ����
    isReady = false;
    boundingRadius = 1.f;  // �浹 ����
    forwardVector.x = 0.f;
    forwardVector.y = 0.f;
    forwardVector.z = 0.f;
    moveDir.x = 0.f;
    moveDir.y = 0.f;
    moveDir.z = 0.f;
    // mass = 2.f;          // ����
    // gravity(9.8f),       // �߷� ����
    //bulletPool(30)

}

void Character::Move(char key)
{
    // g_mazeMap�� ��� ���¸� �����ϴ� static ����
    static bool hasPrinted = false;

    // ó�� �� ���� g_mazeMap ���
    if (!hasPrinted)
    {
        const float blockDiameter = 5.0f; // ����� ����
        //std::cout << "Current Maze Map:" << std::endl;

        for (int z = 0; z < g_mazeMap.size(); ++z)
        {
            for (int x = 0; x < g_mazeMap[z].size(); ++x)
            {
                if (g_mazeMap[z][x] == 1) // ���� ���
                {
                    // ȭ����� ��ǥ (���� ��ǥ)
                    float blockCenterX = x * blockDiameter;
                    float blockCenterZ = z * blockDiameter;
                   /* std::cout << "Wall at Maze Coordinates (" << z << ", " << x << ") -> "
                        << "World Coordinates (" << blockCenterX << ", " << blockCenterZ << ")" << std::endl;*/
                }
            }
        }
        std::cout << "-------------------------" << std::endl;

        // ��� ���� ������Ʈ
        hasPrinted = true;
    }
    // �ӽ� �̵� ���� ���
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

    // === moveDir ����ȭ ===
    float length = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y + moveDir.z * moveDir.z);
    if (length > 0.0f)
    {
        moveDir.x /= length;
        moveDir.z /= length;
    }

    // �ӽ� ��ġ ���
    float newX = position.x + moveDir.x * moveSpeed;
    float newZ = position.z + moveDir.z * moveSpeed;

    // �浹 �˻�: ��� ��Ͽ� ���� ���� �˻�
    const int mazeRows = g_mazeMap.size();
    const int mazeCols = g_mazeMap[0].size();
    bool collision = false;

    for (int z = 0; z < mazeRows; ++z)
    {
        for (int x = 0; x < mazeCols; ++x)
        {
            if (g_mazeMap[z][x] == 1) // ���� ���
            {
                float wallX = x * 5.f; // ���� X ��ǥ
                float wallZ = z * 5.f; // ���� Z ��ǥ

                // === �浹 ���� ��� ===
                float halfBlockSize = 2.5f; // ��� �� ���� ���� ũ�� (3.0 / 2)
                if (std::abs(newX - wallX) < (halfBlockSize + boundingRadius) && // X�� �浹
                    std::abs(newZ - wallZ) < (halfBlockSize + boundingRadius))   // Z�� �浹
                {
                    collision = true; // �浹 �߻�
                    std::cout << "Collision detected with wall at (" << x << ", " << z << ")" << std::endl;
                    break; // �浹�� �߻������Ƿ� �� �̻� �˻����� ����
                }
            }
        }
        if (collision)
            break;
    }
    // �浹�� ���� ��쿡�� ��ġ ����
    if (!collision)
    {
        position.x = newX;
        position.z = newZ;
    }

    // moveDir �ʱ�ȭ
    moveDir.x = 0.0f;
    moveDir.y = 0.0f;
    moveDir.z = 0.0f;

    //cout << "characterPos" << position.x << ", " << position.z << endl;
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
    // �Ѿ��� �ʱ� ��ġ�� ���� (ĳ������ ��ġ + forwardVector * �Ÿ�)
    vec3 bulletPos = vec3{
        position.x + forwardVector.x * 1.0f, // �ѱ��� x ��ǥ
        position.y + forwardVector.y * 1.0f, // �ѱ��� y ��ǥ
        position.z + forwardVector.z * 1.0f  // �ѱ��� z ��ǥ
    };

    vec3 bulletForward = vec3{
       forwardVector.x, // �ѱ��� x ��ǥ
       forwardVector.y, // �ѱ��� y ��ǥ
       forwardVector.z// �ѱ��� z ��ǥ
    };

    auto bullet = bulletPool.GetBullet(); // ��Ȱ��ȭ�� �Ѿ� ��������
    if (bullet) {
        bullet->Activate(bulletPos, bulletForward); // Ȱ��ȭ �� �ʱ�ȭ
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
