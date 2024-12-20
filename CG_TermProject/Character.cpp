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
}

glm::vec3 Character::GetForwardVector() const
{
    return forwardVector2;
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
    forwardVector2.x = cos(glm::radians(yaw));
    forwardVector2.y = 0.f; // ĳ���ʹ� ���� ���⸸ ȸ��
    forwardVector2.z = sin(glm::radians(yaw));
    forwardVector2 = glm::normalize(forwardVector2);
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

    bulletPool.UpdateAllBullets(deltaTime);

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


void Character::Shoot() 
{
    glm::vec3 bulletPos = position + forwardVector2 * 1.5f + glm::vec3(0.0f, 1.0f, 0.0f); // �Ѿ� ��ġ

    auto bullet = bulletPool.GetBullet(); // ��Ȱ��ȭ�� �Ѿ� ��������
    if (bullet) {
        bullet->Activate(bulletPos, forwardVector2); // Ȱ��ȭ �� �ʱ�ȭ
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
















void Character::Render(GLuint shaderProgram)
{
    glBindVertexArray(vao);

    // �� ��ȯ ��� ���
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), position); // �̵� ��ȯ
    glm::mat4 size = glm::scale(glm::mat4(1.0f), scale);             // ������ ��ȯ

    // forwardVector�� ����� ȸ�� ��ȯ
    glm::vec3 up(0.0f, 1.0f, 0.0f);                                  // ���� �� ����
    glm::vec3 right = glm::normalize(glm::cross(up, forwardVector2)); // ������ ����
    glm::vec3 correctedUp = glm::normalize(glm::cross(forwardVector2, right)); // ������ ���� ����

    glm::mat4 rotate = glm::mat4(1.0f);
    rotate[0] = glm::vec4(right, 0.0f);        // ������ ����
    rotate[1] = glm::vec4(correctedUp, 0.0f); // ���� ����
    rotate[2] = glm::vec4(forwardVector2, 0.0f); // ���� ����

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