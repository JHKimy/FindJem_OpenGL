#include "Bullet.h"
#include<iostream>

Bullet::Bullet(
    const glm::vec3& position, 
    const glm::vec3& direction, 
    float speed)
    : Actor(
        "Sphere.obj", 
        position, 
        glm::vec3(0.1f), 
        glm::vec3(0.0f), 
        glm::vec3(1.0f)),
    direction(glm::normalize(direction)), 
    isActive(true), 
    moveSpeed(speed) 
{
    std::cout << "Bullet object created!" << std::endl;
}

void Bullet::Update(float deltaTime) {
    if (isActive) {
        position += direction * moveSpeed * deltaTime;
    }
}

bool Bullet::IsActive() const {
    return isActive;
}

void Bullet::Deactivate() {
    isActive = false;
}

bool Bullet::CheckCollision(const glm::vec3& targetPos, float targetRadius)
{
    float distance = glm::length(position - targetPos);
    return distance < targetRadius;
}
