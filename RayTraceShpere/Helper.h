#pragma once
#include <cstdlib>

typedef struct vec3{
	float e[3];
} vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline float random_float() {
	// Returns a random real in [0, 1)
	return rand() / (RAND_MAX + 1.0);
}

inline float random_float(float min, float max) {
	// Returns a random real in [min, max)
	return min + (max - min) * random_float();
}

inline vec3 random_vec3(float min, float max)
{
	vec3 result = { random_float(min, max), random_float(min, max) , random_float(min, max) };
	return result;
}
