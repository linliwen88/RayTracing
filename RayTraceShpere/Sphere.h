#pragma once
#include <glm/glm.hpp>
#include "hittable.h"

class sphere : public hittable
{
public:
	sphere(glm::vec3 origin, float radius, int material) { Origin = origin; Radius = radius; Material = material; }
	~sphere() override {}

	int Material;
	float Radius;
	glm::vec3 Origin;
};