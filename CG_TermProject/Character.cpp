#include "Character.h"
#include "Global.h"
#include "Bullet.h"
#include <algorithm>

Character::Character(const glm::vec3& position)
    : Actor(
        "Boss.obj",         // obj 
        position,           // 위치
        glm::vec3(0.05f),   // 크기 
        glm::vec3(0),       // 회전
        glm::vec3(0, 1, 0)  // 색상
    ),

    health(100),        // 체력
    mass(2.f),          // 질량
    moveSpeed(0.5f),    // 스피드
    isJumping(false),   // 점프 상태
    gravity(9.8f),       // 중력 영향
    bulletPool(30)
{
    boundingRadius = 1.5f;  // 충돌 범위
    
    
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

    // 전방 벡터 업데이트 (Pitch는 항상 0)
    forwardVector.x = cos(glm::radians(yaw));
    forwardVector.y = 0.f; // 캐릭터는 수평 방향만 회전
    forwardVector.z = sin(glm::radians(yaw));
    forwardVector = glm::normalize(forwardVector);
}

void Character::Jump() {
    if (!isJumping) {
        isJumping = true;
        jumpSpeed = 10.f; // 초기 점프 속도
    }
}

void Character::Update(float deltaTime) {

    // SetRotation(forwardVector);

    // 중력 적용
    if (isJumping)
    {
        position.y += jumpSpeed * deltaTime;
        jumpSpeed -= gravity * mass * deltaTime;

        // 땅에 내려올때
        if (position.y <= 0.0f) {
            position.y = 0.0f;
            isJumping = false;
        }
    }

    bulletPool.UpdateAllBullets(deltaTime, GetPosition());

    //// 총알 업데이트
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
    glm::vec3 bulletPos = position + forwardVector * 1.5f + glm::vec3(0.0f, 1.0f, 0.0f); // 총알 위치

    auto bullet = bulletPool.GetBullet(); // 비활성화된 총알 가져오기
    if (bullet) {
        bullet->Activate(bulletPos, forwardVector); // 활성화 및 초기화
    }
}

//void Character::UpdateBullets(float deltaTime)
//{
//    for (auto& bullet : bullets) {
//        bullet->Update(deltaTime);
//    }
//
//    //// 비활성화된 총알 제거
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

    //// VAO 바인딩 상태 출력
    //std::cout << "VAO ID: " << vao << std::endl;

    //// vertices 크기 출력
    //std::cout << "Vertices Count: " << vertices.size() << std::endl;

    //// Position 출력
    //std::cout << "Character Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;

    //// Forward Vector 출력
    //std::cout << "Forward Vector: " << forwardVector.x << ", " << forwardVector.y << ", " << forwardVector.z << std::endl;
    glBindVertexArray(vao);

    // 모델 변환 행렬 계산
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), position); // 이동 변환
    glm::mat4 size = glm::scale(glm::mat4(1.0f), scale);             // 스케일 변환

    // forwardVector를 사용한 회전 변환
    glm::vec3 up(0.0f, 1.0f, 0.0f);                                  // 월드 업 벡터
    glm::vec3 right = glm::normalize(glm::cross(up, forwardVector)); // 오른쪽 벡터
    glm::vec3 correctedUp = glm::normalize(glm::cross(forwardVector, right)); // 수정된 위쪽 벡터

    glm::mat4 rotate = glm::mat4(1.0f);
    rotate[0] = glm::vec4(right, 0.0f);        // 오른쪽 벡터
    rotate[1] = glm::vec4(correctedUp, 0.0f); // 위쪽 벡터
    rotate[2] = glm::vec4(forwardVector, 0.0f); // 전방 벡터

    // 합성 변환 행렬
    glm::mat4 TRANS = translate * rotate * size;

    GLuint modelLoc = glGetUniformLocation(shaderProgram, "modelTransform");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TRANS));

    // 색상 전달
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLoc, color.r, color.g, color.b);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);


    //// 총알 렌더링
    //for (const auto& bullet : bullets) {
    //    bullet->Render(shaderProgram);
    //}

}

void Character::SetForwardVector(glm::vec3 forwardVec)
{
    forwardVector = forwardVec;
}
