#pragma once
#include "Types.h"
#include <vector>
#include <memory> // 스마트 포인터 사용


class Bullet
{
private:
	bool isActive;
	vec3 position;           // 현재 위치
	vec3 direction;
	float moveSpeed;

public:
	Bullet();

	void Update();
	bool IsActive();
	void DeActivate();
	void Activate(vec3& position, vec3& newDirection);




	vec3 GetDirection()
	{
		return direction;
	}


	// 위치를 설정하는 함수
	void SetPosition(vec3& NewPos)
	{
		position = NewPos;
	}

	vec3& GetPosition()
	{
		return position;
	}


	float boundingRadius = 0.3f;
};



// 오브젝트 풀
class BulletPool
{
public:
	std::vector<std::shared_ptr<Bullet>> pool;
	size_t poolSize;

public:
	BulletPool() = default;
	BulletPool(size_t poolSize);

	// 비활성화 된 총알 반환
	std::shared_ptr<Bullet> GetBullet();
	void UpdateAllBullets(vec3 startPos);
	std::vector<std::shared_ptr<Bullet>>& GetAllBullets();

	// 모든 총알 반환
	//const std::vector<std::shared_ptr<Bullet>>& GetAllBullets() const;

	// 사용 가능한 총알(비활성화된 총알) 개수 반환
	size_t GetAvailableBulletCount();

};