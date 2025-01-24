#pragma once
#include <cmath>

// 벡터 구조체 정의
struct vec3 {
    float x, y, z;
    
    vec3() = default; // 기본 생성자

    // 2개의 매개변수를 받는 생성자
    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}


};

struct vec2 {
    float x, z;

    vec2() = default; // 기본 생성자

    // 2개의 매개변수를 받는 생성자
    vec2(float _x, float _z) : x(_x), z(_z) {}
};

// 전역 함수 정의를 네임스페이스로 감싸기
namespace V {

    // 3D 벡터 간 거리 계산 함수
    inline float Distance( vec3& startPos,  vec3& endPos)
    {
        float dx = endPos.x - startPos.x;
        float dy = endPos.y - startPos.y;
        float dz = endPos.z - startPos.z;

        // 거리 계산 공식: sqrt(dx^2 + dy^2 + dz^2)
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    // 벡터 정규화 함수
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

extern int g_totalDefeatedEnemies ; // 모든 캐릭터의 적 처치 합계

extern bool g_reachExit;

extern vec2 g_exitPosition;