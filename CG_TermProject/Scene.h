#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "stdafx.h"

class Scene
{
private:
    //void InitializeCubeBuffers();             // ���� �ʱ�ȭ
    //void InitializeEnemies();               // �� �ʱ�ȭ
    //void UpdateEnemies(float deltaTime);    // �� ������Ʈ
    //void CheckCollisions();                 // �浹 �˻�
    //void DrawFloor();                       // �ٴ� �׸���
    //void DrawCharacter();                   // ĳ���� �׸���
    //void DrawEnemies();                     // �� �׸���
    //void DrawBullets();                     // �Ѿ� �׸���


public:
    Scene();
    void Initialize();   // �ʱ�ȭ
    void Update(float deltaTime); // ������Ʈ
    void Render();       // ������
    void Shutdown();     // ���ҽ� ����


};

#endif