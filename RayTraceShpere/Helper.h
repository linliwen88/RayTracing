#pragma once
#include <cmath>
#include <cstdlib>

//typedef struct vec3{
//	float e[3];
//} vec3;

inline float random_float() {
	// Returns a random real in [0, 1)
	return rand() / (RAND_MAX + 1.0);
}

inline float random_float(float min, float max) {
	// Returns a random real in [min, max)
	return min + (max - min) * random_float();
}

class vec3 {
public:
	float e[3];

	vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}
	vec3(vec3& u) {}

	vec3 operator=(const vec3& u)
	{
		return *this;
	}

	operator glm::vec3()
	{
		return glm::vec3(e[0], e[1], e[2]);
	}

	float length() const
	{
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}

	static vec3 random()
	{
		return vec3(random_float(), random_float(), random_float());
	}

	static vec3 random(float min, float max)
	{
		return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
	}
};

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}


//inline vec3 random_vec3(float min, float max)
//{
//	vec3 result( random_float(min, max), random_float(min, max) , random_float(min, max) );
//	return result;
//}
