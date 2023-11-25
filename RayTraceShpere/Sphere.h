#pragma once
#include <glm/glm.hpp>
#include "hittable.h"

class sphere : public hittable
{
public:
	glm::vec3 Origin;
	float Radius;

	sphere(glm::vec3 origin, float radius) { Origin = origin; Radius = radius; }
	~sphere() override {}
};