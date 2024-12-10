#pragma once

//#include "stdafx.h"
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Actor.h"
#include "Character.h"
#include "Enemy.h"

class Scene
{
private:
    Camera mainCamera;  // ī�޶�
    Light mainLight;    // ����
    std::vector<Actor*> actors; // ���� ���Ե� Actor ��ü��
    GLuint shaderProgram;   // ���̴� ���α׷� ID
    Character* mainCharacter;  // �÷��̾� ĳ����
    //Enemy* enemy;   // �� ��ü
    std::vector<Enemy*> enemeys;    // �� ��ü��

    //void InitializeCubeBuffers();             // ���� �ʱ�ȭ
    //void InitializeEnemies();               // �� �ʱ�ȭ
    //void UpdateEnemies(float deltaTime);    // �� ������Ʈ
    //void CheckCollisions();                 // �浹 �˻�
    //void DrawFloor();                       // �ٴ� �׸���
    //void DrawCharacter();                   // ĳ���� �׸���
    //void DrawEnemies();                     // �� �׸���
    //void DrawBullets();                     // �Ѿ� �׸���


public:
    // ������
    Scene(GLuint shaderProgram);
    void Initialize();   // �ʱ�ȭ
    void Update(float deltaTime); // ������Ʈ
    void Render();       // ������
    void Shutdown();     // ���ҽ� ����
    Character* GetCharacter();

};