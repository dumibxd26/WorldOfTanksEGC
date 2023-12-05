#pragma once
#include <glm/glm.hpp>
#include <vector>

class Projectile
{
public:
	float radius;
	glm::vec3 position;
	float appear_time;
	float angle = 0;
	bool isEnemy;

	Projectile(float radius, glm::vec3 position, bool isEnemy);
};

