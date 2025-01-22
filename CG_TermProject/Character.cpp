#include "Character.h"
#include "Global.h"
#include "Bullet.h"
#include <algorithm>

Character::Character(const glm::vec3& position)
    : Actor(
        "Boss.obj",         // obj 
        position,           // ��ġ
        glm::vec3(0.05f),   // ũ�� 
        glm::vec3(0),       // ȸ��
        glm::vec3(0, 1, 0)  // ����
    ),

    health(100),        // ü��
    mass(2.f),          // ����
    moveSpeed(0.5f),    // ���ǵ�
    isJumping(false),   // ���� ����
    gravity(9.8f),       // �߷� ����
    bulletPool(30)
{
    boundingRadius = 1.5f;  // �浹 ����
    
    
    bullets.resize(5);
}

glm::vec3 Character::GetForwardVector() const
{
    return forwardVector;
}

void Character::Move(const glm::vec3& dir)
{
    glm::vec3 direction;

    direction = glm::normalize(dir);
    position += direction * moveSpeed;
}

void Character::Rotate(float deltaYaw)
{
    yaw += deltaYaw;
    if (yaw > 360.0f) yaw -= 360.0f;
    if (yaw < 0.0f) yaw += 360.0f;

    // ���� ���� ������Ʈ (Pitch�� �׻� 0)
    forwardVector.x = cos(glm::radians(yaw));
    forwardVector.y = 0.f; // ĳ���ʹ� ���� ���⸸ ȸ��
    forwardVector.z = sin(glm::radians(yaw));
    forwardVector = glm::normalize(forwardVector);
}

void Character::Jump() {
    if (!isJumping) {
        isJumping = true;
        jumpSpeed = 10.f; // �ʱ� ���� �ӵ�
    }
}

void Character::Update(float deltaTime) {

    // SetRotation(forwardVector);

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

    bulletPool.UpdateAllBullets(deltaTime, GetPosition());

    //// �Ѿ� ������Ʈ
    //for (auto bullet : bullets) {
    //    bullet->Update(deltaTime);
    //}
}



void Character::TakeDamage(int amount) {
    health -= amount;
}

bool Character::IsGameOver() const {
    return health <= 0;
}

float Character::GetMoveSpeed()
{
    return moveSpeed;
}

void Character::SetMoveSpeed(float speed)
{
    moveSpeed = speed;
}

BulletPool& Character::GetBulletPool()
{
    return bulletPool;
}

int Character::GetDefeatedEnemies() const {
    return defeatedEnemies;
}

void Character::IncrementDefeatedEnemies() {
    defeatedEnemies++;
}

void Character::Shoot()
{
    glm::vec3 bulletPos = position + forwardVector * 1.5f + glm::vec3(0.0f, 1.0f, 0.0f); // �Ѿ� ��ġ

    auto bullet = bulletPool.GetBullet(); // ��Ȱ��ȭ�� �Ѿ� ��������
    if (bullet) {
        bullet->Activate(bulletPos, forwardVector); // Ȱ��ȭ �� �ʱ�ȭ
    }
}

//void Character::UpdateBullets(float deltaTime)
//{
//    for (auto& bullet : bullets) {
//        bullet->Update(deltaTime);
//    }
//
//    //// ��Ȱ��ȭ�� �Ѿ� ����
//    //bullets.erase(
//    //    std::remove_if(bullets.begin(), bullets.end(),
//    //        [](const std::unique_ptr<Bullet>& bullet) { return !bullet->IsActive(); }),
//    //    bullets.end()
//    //);
//}
//std::vector<std::unique_ptr<Bullet>>& Character::GetBullets()
//{
//    return bullets;
//}
//void Character::Stop()
//{
//    moveSpeed = 0.f;
//}
//
//void Character::ResetSpeed()
//{
//    moveSpeed = 0.05f;
//}
//const std::vector<Bullet*>& Character::GetBullets() const {
//    return bullets;
//}
//glm::vec3 Character::GetPosition()
//{
//    return glm::vec3();
//}
//void Character::SetDirection(const glm::vec3& dir) {
//    direction = glm::normalize(dir);
//}
//
//glm::vec3 Character::GetDirection() const {
//    return direction;
//}

float Character::GetYaw() const
{
    return yaw;
}

void Character::SetYaw(float _yaw)
{
    yaw = _yaw;
}

void Character::Render(GLuint shaderProgram)
{
    //std::cout << "Rendering Character..." << std::endl;

    //// VAO ���ε� ���� ���
    //std::cout << "VAO ID: " << vao << std::endl;

    //// vertices ũ�� ���
    //std::cout << "Vertices Count: " << vertices.size() << std::endl;

    //// Position ���
    //std::cout << "Character Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;

    //// Forward Vector ���
    //std::cout << "Forward Vector: " << forwardVector.x << ", " << forwardVector.y << ", " << forwardVector.z << std::endl;
    glBindVertexArray(vao);

    // �� ��ȯ ��� ���
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), position); // �̵� ��ȯ
    glm::mat4 size = glm::scale(glm::mat4(1.0f), scale);             // ������ ��ȯ

    // forwardVector�� ����� ȸ�� ��ȯ
    glm::vec3 up(0.0f, 1.0f, 0.0f);                                  // ���� �� ����
    glm::vec3 right = glm::normalize(glm::cross(up, forwardVector)); // ������ ����
    glm::vec3 correctedUp = glm::normalize(glm::cross(forwardVector, right)); // ������ ���� ����

    glm::mat4 rotate = glm::mat4(1.0f);
    rotate[0] = glm::vec4(right, 0.0f);        // ������ ����
    rotate[1] = glm::vec4(correctedUp, 0.0f); // ���� ����
    rotate[2] = glm::vec4(forwardVector, 0.0f); // ���� ����

    // �ռ� ��ȯ ���
    glm::mat4 TRANS = translate * rotate * size;

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TRANS));

    // ���� ����
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLoc, color.r, color.g, color.b);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);


    //// �Ѿ� ������
    //for (const auto& bullet : bullets) {
    //    bullet->Render(shaderProgram);
    //}

}

void Character::SetForwardVector(glm::vec3 forwardVec)
{
    forwardVector = forwardVec;
}
