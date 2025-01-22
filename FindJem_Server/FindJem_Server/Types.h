#pragma once

struct vec3 {
    float x, y, z;
};

struct vec2 {
    float x, z;

    vec2() = default; // �⺻ ������

    // 2���� �Ű������� �޴� ������
    vec2(float x_val, float z_val) : x(x_val), z(z_val) {}
};
