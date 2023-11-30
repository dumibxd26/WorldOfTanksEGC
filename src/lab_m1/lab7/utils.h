#pragma once
#include "utils/glm_utils.h"

inline glm::mat4 rotateOY(float radians) {
	return glm::mat4(
		glm::vec4(cos(radians), 0, sin(radians), 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(-sin(radians), 0, cos(radians), 0),
		glm::vec4(0, 0, 0, 1)
	);
}