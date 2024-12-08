#include "Enemy.h"

Enemy::Enemy(
    const std::string& objFilePath, 
    const glm::vec3& position, 
    const glm::vec3& scale,
    const glm::vec3& rotation, 
    const glm::vec3& color, 
    float speed, 
    int health)
    : 
    Actor(
        objFilePath, 
        position, 
        scale, 
        rotation, 
        color), 
    moveSpeed(speed), 
    health(health), 
    isActive(true) {}

void Enemy::Move() {
    if (isActive) {
        position += direction * moveSpeed;
    }
}

void Enemy::TakeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        isActive = false;
    }
}

bool Enemy::IsActive() const {
    return isActive;
}
