#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "components/simple_scene.h"
#include "TankComponent.h"
#include "Projectile.h"

class Tank
{
public:

	glm::vec3 bottom_position;
	float color_intensity;
	float rotation_angle;
	int goesPositive; // 1 if positive, -1 if negative, depeonds on rotation angle
	float lives;
	int translation_speed = 1;
	int rotation_speed = 100;
	float radius;
	bool isInvincible = false;
	float invincibleTime = 5;
	bool isEnemy;

	float steps = 0;

	TankComponent* head;
	TankComponent* cannon;
	TankComponent* tracks;
	TankComponent* base;
	// contains the components above
	std::vector<TankComponent*> components;
	float last_projectile_time = 0;
	std::vector<Projectile *> projectiles;

	Tank(glm::vec3 bottom_position, float color_intensity, float rotation_angle, TankComponent* head, TankComponent* cannon, TankComponent* tracks, TankComponent* base, bool isEnemy);
};

