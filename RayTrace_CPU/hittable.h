#pragma once

#include "ray.h"

class material;

class hit_record {
public:
	point3 p;
	double t;
	vec3 normal;
	bool front_face;
	shared_ptr<material> mat;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		// set the hit record normal vector
		// note: `outward_normal` is assumed to have unit length

		front_face = dot(r.direction(), outward_normal) < 0.0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};