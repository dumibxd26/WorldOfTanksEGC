#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class TankComponent
{
public:
	std::string name;
	glm::vec3 bottom_position;
	glm::vec3 color;
	float size_factor;
	float rotation_angle;
	float default_rotation_angle;
	float default_scale_factor;

	// -1 0 1
	int rotationX_Y, rotationY_Y, rotationZ_Y;

	TankComponent(std::string name, glm::vec3 bottom_position, glm::vec3 color, float size_factor, float default_rotation_angle, float default_scale_factor,
		int rotationX_Y, int rotationY_Y, int rotationZ_Y);

};

