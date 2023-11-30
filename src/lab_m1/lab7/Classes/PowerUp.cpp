#include "PowerUp.h"

PowerUp::PowerUp(glm::vec3 bottom_position, int type)
{
	this->bottom_position = bottom_position;
	this->color = color;
	this->type = type;

	switch (type) {
		case 0: // heart
			this->color = glm::vec3(1, 0, 0);
			break;
		case 1: // star
			this->color = glm::vec3(1, 1, 0);
			break;
		case 2: // thunder
			this->color = glm::vec3(0, 1, 0);
			break;
	}
}