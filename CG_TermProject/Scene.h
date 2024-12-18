#pragma once

#include <vector>
#include <memory> // ����Ʈ ������ ���
#include "MazeGenerator.h" // Scene.h�� ���� ���� (�� �����)
#include <GL/glew.h> // OpenGL�� GLuint �� ���� Ÿ�� ����
//#include "Enemy.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

// ���� ����
class Camera;
class Light;
class Actor;
class Character;
//class Enemy;
class MazeGenerator;

class Scene
{
private:
    // ���̴� ���α׷� ID
    GLuint SceneShader;                     
    
    // ī�޶�
    std::unique_ptr<Camera> mainCamera;       
    // ���� 
    std::unique_ptr<Light> mainLight;         
    // ���� ���Ե� Actor ��ü��
    std::vector<std::unique_ptr<Actor>> actors; 
    // �÷��̾� ĳ����
    std::unique_ptr<Character> mainCharacter; 
    // �̷� ������
    std::unique_ptr<MazeGenerator> mazeGenerator; 
    
    //std::vector<std::unique_ptr<class Enemy>> enemies; // �� ��ü��

    // �̷� ������
    vector<vector<int>> mazeMap;    
    // �̷ε����͸� Actor�� ��ȯ
    void InitializeMaze();          

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