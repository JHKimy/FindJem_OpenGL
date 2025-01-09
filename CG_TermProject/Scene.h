#pragma once

#include <vector>
#include <memory> // ����Ʈ ������ ���
#include <GL/glew.h> // OpenGL�� GLuint �� ���� Ÿ�� ����
#include "MazeGenerator.h" // �̷� ������
#include "array"
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

    // ===== ��� ���� =====
    GLuint SceneShader; // ���̴� ���α׷� ID

    // �ֿ� ���� ��ü
    std::unique_ptr<Camera> mainCamera;         // ���� ī�޶�
    std::unique_ptr<Light> mainLight;           // ���� ����
    std::unique_ptr<Character> mainCharacter;   // �÷��̾� ĳ����
    
    
    std::unique_ptr<Character> otherCharacter1;   // �÷��̾� ĳ����
    std::unique_ptr<Actor> otherCharacter2;   // �÷��̾� ĳ����

    
    //std::unique_ptr<MazeGenerator> mazeGenerator; // �̷� ������

    // ���Ϳ� �� ����
    std::vector<std::unique_ptr<Actor>> actors; // ���� ���Ե� ���͵�
    
    
    
    
public:
    std::array<std::shared_ptr<Enemy>,1> enemies; // ����


private:





    std::vector<std::vector<int>> mazeMap; // �̷� ������

    glm::vec3 blockSize; // ��� ũ��


    // ===== ���� ��ƿ��Ƽ �Լ� =====
    void InitializeMaze();       // �̷� �ʱ�ȭ
    void InitializeEnemies();    // �� �ʱ�ȭ
    void HandleBulletEnemyCollisions(); // ���� �Ѿ� �浹 ó��
    void RemoveInactiveEnemies();   // ��Ȱ��ȭ �� �� �����

public:
    // ===== ������ �� �Ҹ��� =====
    explicit Scene(GLuint shaderProgram); // ������
    ~Scene() = default; // �Ҹ���


    // ===== �ֿ� ��� �Լ� =====
    void Initialize();           // �ʱ�ȭ
    void Update(float deltaTime); // ������Ʈ
    void Render();               // ������


    // ===== ������ �Լ� =====
    Character* GetCharacter() { return mainCharacter.get(); }
    Character* GetOtherCharacter1() { return otherCharacter1.get(); }

    Camera* GetCamera() { return mainCamera.get(); }
    const std::vector<std::unique_ptr<Actor>>& GetActors() const { return actors; };
    const std::vector<std::vector<int>>& GetMazeMap() const { return mazeMap; } 



    // ���� �̷���ǥ �޾ƿ���
    void SetMaze(int mazeMapData[15][15]);


    //void AddEnemy(float x, float y, float z);
};
