#include "Tank.h"

Tank::Tank(glm::vec3 bottom_position, float color_intensity, float rotation_angle, TankComponent* head, TankComponent* cannon, TankComponent* tracks, TankComponent* base, bool isEnemy) {
	this->bottom_position = bottom_position;
	this->color_intensity = color_intensity;
	this->rotation_angle = rotation_angle;
	this->head = head;
	this->cannon = cannon;
	this->tracks = tracks;
	this->base = base;
	this->lives = 3;
	this->goesPositive = 1;
	this->components.push_back(head);
	this->components.push_back(cannon);
	this->components.push_back(base);
	this->components.push_back(tracks);
	this->isEnemy = isEnemy;
}