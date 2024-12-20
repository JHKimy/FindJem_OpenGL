#pragma once

#include <vector>
#include <memory> // ����Ʈ ������ ���
#include "MazeGenerator.h" // Scene.h�� ���� ���� (�� �����)
#include <GL/glew.h> // OpenGL�� GLuint �� ���� Ÿ�� ����
#include "Enemy.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

// ���� ����
class Camera;
class Light;
class Actor;
class Character;
class Enemy;
class MazeGenerator;

class Scene
{
private:
    // ���̴� ���α׷� ID
    GLuint SceneShader;

    // ī�޶�
    unique_ptr<Camera> mainCamera;
    // ���� 
    unique_ptr<Light> mainLight;
    // ���� ���Ե� Actor ��ü��
    vector<unique_ptr<Actor>> actors;
    // �÷��̾� ĳ����
    unique_ptr<Character> mainCharacter;
    // �̷� ������
    unique_ptr<MazeGenerator> mazeGenerator;

    vector<unique_ptr<Enemy>> enemies; // �� ��ü��

    // �̷� ������
    vector<vector<int>> mazeMap;
    // �̷ε����͸� Actor�� ��ȯ
    void InitializeMaze();

    glm::vec3 blockSize; // ��� ũ��


    // �� ��ȯ
    void InitializeEnemies();

public:
    // ������ �� �Ҹ���
    Scene(GLuint shaderProgram);
    // ����ũ �����Ͱ� �ڵ����� �޸� ����
    ~Scene() = default;

    void Initialize();
    void Update(float deltaTime);
    void Render();

    Character* GetCharacter();
    Camera* GetCamera();
    const std::vector<std::unique_ptr<Actor>>& GetActors() const;
};