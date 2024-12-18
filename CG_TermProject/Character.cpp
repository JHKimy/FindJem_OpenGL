#include "Character.h"
#include "Global.h"

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
        gravity(9.8f)       // 중력 영향
{
        boundingRadius = 1.5f;  // 충돌 범위
}

void Character::Move(const glm::vec3& dir) 
{
    glm::vec3 direction;

    direction = glm::normalize(dir);
    position += direction * moveSpeed;
}

void Character::Rotate(glm::vec3 forwardVector)
{
    SetRotation(forwardVector);
}

void Character::Jump() {
    if (!isJumping) {
        isJumping = true;
        jumpSpeed = 5.f; // 초기 점프 속도
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

    //// 총알 업데이트
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

float Character::GetMoveSpeed()
{
    return moveSpeed;
}

void Character::SetMoveSpeed(float speed)
{
    moveSpeed = speed;
}

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
    return rotation.y; // Y축 회전 값 반환
}















void Character::Render(GLuint shaderProgram)
{
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
}
