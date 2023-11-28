#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "hittable.h"
#include "material.h"

class sphere : public hittable
{
public:
	sphere(glm::vec3 origin, float radius, std::shared_ptr<material> mat) { Origin = origin; Radius = radius; Material = mat; }
	~sphere() override {}

	std::shared_ptr<material> Material;
	float Radius;
	glm::vec3 Origin;
};