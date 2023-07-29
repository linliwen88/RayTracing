#pragma once
#include <glm/glm.hpp>
class Sphere
{
public:
	glm::vec3 Origin;
	float Radius;

	Sphere(glm::vec3 origin, float radius) { Origin = origin; Radius = radius; }
};