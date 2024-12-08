#pragma once
#include "Actor.h"

class Enemy : public Actor {
private:
    int health;
    bool isActive;
    glm::vec3 direction;
    float moveSpeed;

public:
    Enemy(const std::string& objFilePath, const glm::vec3& position, const glm::vec3& scale,
        const glm::vec3& rotation, const glm::vec3& color, float speed = 0.1f, int health = 3);

    void Move();
    void TakeDamage(int amount);
    bool IsActive() const;
};
