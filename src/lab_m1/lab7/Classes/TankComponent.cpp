#include "TankComponent.h"

// define the constructor

TankComponent::TankComponent(std::string name, glm::vec3 bottom_position, glm::vec3 color, float size_factor, float default_rotation_angle, float default_scale_factor,
	int rotationX_Y, int rotationY_Y, int rotationZ_Y)
{
	this->name = name;
	this->bottom_position = bottom_position;
	this->color = color;
	this->size_factor = size_factor;
	this->rotation_angle = 0;
	this->default_rotation_angle = default_rotation_angle;
	this->default_scale_factor = default_scale_factor;
	this->rotationX_Y = rotationX_Y;
	this->rotationY_Y = rotationY_Y;
	this->rotationZ_Y = rotationZ_Y;
}