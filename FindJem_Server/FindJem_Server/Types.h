#pragma once
#include <cmath>

// ���� ����ü ����
struct vec3 {
    float x, y, z;
    
    vec3() = default; // �⺻ ������

    // 2���� �Ű������� �޴� ������
    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}


};

struct vec2 {
    float x, z;

    vec2() = default; // �⺻ ������

    // 2���� �Ű������� �޴� ������
    vec2(float _x, float _z) : x(_x), z(_z) {}
};

// ���� �Լ� ���Ǹ� ���ӽ����̽��� ���α�
namespace V {

    // 3D ���� �� �Ÿ� ��� �Լ�
    inline float Distance( vec3& startPos,  vec3& endPos)
    {
        float dx = endPos.x - startPos.x;
        float dy = endPos.y - startPos.y;
        float dz = endPos.z - startPos.z;

        // �Ÿ� ��� ����: sqrt(dx^2 + dy^2 + dz^2)
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    // ���� ����ȭ �Լ�
    inline vec3 Normalize( vec3& v)
    {
        float magnitude = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

        if (magnitude == 0.0f)
        {
            return vec3{ 0.0f, 0.0f, 0.0f };
        }

        return vec3{ v.x / magnitude, v.y / magnitude, v.z / magnitude };
    }
}

extern int g_totalDefeatedEnemies ; // ��� ĳ������ �� óġ �հ�

extern bool g_reachExit;

extern vec2 g_exitPosition;