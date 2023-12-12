#pragma once
#include <glm/glm.hpp>
#include "Helper.h"

class material {
public:
	material(int _type, glm::vec3 _albedo) : type(_type), albedo(_albedo) {}

	int type;
	glm::vec3 albedo;
};

