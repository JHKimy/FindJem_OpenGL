#pragma once
#include "Actor.h"

class Bullet : public Actor {
private:
    glm::vec3 direction;
    bool isActive;
    float moveSpeed;

public:
    Bullet(const glm::vec3& position, const glm::vec3& direction, float speed = 0.5f);

    void Update(float deltaTime);
    bool IsActive() const;
    void Deactivate();
    bool CheckCollision(const glm::vec3& targetPos, float targetRadius);
};
