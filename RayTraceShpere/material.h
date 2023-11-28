#pragma once
#include <glm/glm.hpp>

class material {
public:
	material(int _type, glm::vec3 _albedo) : type(_type), albedo(_albedo) {}

	int type;
	glm::vec3 albedo;
};
