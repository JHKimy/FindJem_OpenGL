#pragma once

struct vec3 {
    float x, y, z;
};

struct vec2 {
    float x, z;

    vec2() = default; // 기본 생성자

    // 2개의 매개변수를 받는 생성자
    vec2(float x_val, float z_val) : x(x_val), z(z_val) {}
};
