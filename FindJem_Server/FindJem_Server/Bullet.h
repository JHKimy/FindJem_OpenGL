#pragma once
#include "Types.h"
#include <vector>
#include <memory> // ����Ʈ ������ ���


class Bullet
{
private:
	bool isActive;
	vec3 position;           // ���� ��ġ
	vec3 direction;
	float moveSpeed;

public:
	Bullet();

	void Update(float deltaTime);
	bool IsActive();
	void DeActivate();
	void Activate(vec3& position, vec3& newDirection);

	vec3 GetDirection()
	{
		return direction;
	}


	// ��ġ�� �����ϴ� �Լ�
	void SetPosition(vec3& NewPos)
	{
		position = NewPos;
	}

	vec3& GetPosition()
	{
		return position;
	}
};



// ������Ʈ Ǯ
class BulletPool
{
public:
	std::vector<std::shared_ptr<Bullet>> pool;
	int poolSize;

public:
	BulletPool() = default;
	BulletPool(int poolSize);

	// ��Ȱ��ȭ �� �Ѿ� ��ȯ
	std::shared_ptr<Bullet> GetBullet();
	void UpdateAllBullets(float deltaTime, vec3 startPos);
	std::vector<std::shared_ptr<Bullet>>& GetAllBullets();

	// ��� �Ѿ� ��ȯ
	//const std::vector<std::shared_ptr<Bullet>>& GetAllBullets() const;

	// ��� ������ �Ѿ�(��Ȱ��ȭ�� �Ѿ�) ���� ��ȯ
	int GetAvailableBulletCount();

};