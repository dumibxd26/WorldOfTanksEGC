#include "Projectile.h"

Projectile::Projectile(float radius, glm::vec3 position, bool isEnemy) {
	this->radius = radius;
	this->position = position;
	this->isEnemy = isEnemy;
}