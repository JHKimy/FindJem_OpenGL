#include "Character.h"

Character::Character(
    const std::string& objFilePath, 
    const glm::vec3& position, 
    const glm::vec3& scale,
    const glm::vec3& rotation, 
    const glm::vec3& color, 
    float speed, 
    int health)
    : 
    Actor( objFilePath, position, scale, rotation, color),
    
    moveSpeed(speed), 
    direction(glm::vec3(0.f,0.f,-1.f)), 
    health(health), isJumping(false), 
    jumpSpeed(0.2f), gravity(9.8f) 
{
    mass = 2.f;
}

void Character::Move(const glm::vec3& dir) {
    direction = glm::normalize(dir);
    position += direction * moveSpeed;
}

void Character::Jump() {
    if (!isJumping) {
        isJumping = true;
        jumpSpeed = 5.f; // �ʱ� ���� �ӵ�
        printf("dfjslf");
    }
}

void Character::Update(float deltaTime) {
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

    //// �Ѿ� ������Ʈ
    //for (auto bullet : bullets) {
    //    bullet->Update(deltaTime);
    //}
}

void Character::Shoot() {
    // Bullet* bullet = new Bullet(position + glm::vec3(0.0f, 0.7f, 0.0f), direction);
    // bullets.push_back(bullet);
}

void Character::TakeDamage(int amount) {
    health -= amount;
}

bool Character::IsGameOver() const {
    return health <= 0;
}

//const std::vector<Bullet*>& Character::GetBullets() const {
//    return bullets;
//}

void Character::SetDirection(const glm::vec3& dir) {
    direction = glm::normalize(dir);
}

glm::vec3 Character::GetDirection() const {
    return direction;
}
