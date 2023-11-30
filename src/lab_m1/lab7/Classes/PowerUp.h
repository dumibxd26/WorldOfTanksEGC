#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>

class PowerUp
{
public:
	glm::vec3 bottom_position;
	glm::vec3 color;
	float radius = 1;
	int type; // 0 for heart, 1 for star, 3 for thunder

	PowerUp(glm::vec3 bottom_position, int type);
};

