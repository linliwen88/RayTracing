#pragma once

#include "common.h"

class hit_record;

class material {
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
	virtual color get_albedo() const = 0; // The fraction of light that a surface reflects
};

class lambertian : public material {
public:
	lambertian(const color& a) : albedo(a) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 scatter_direction = rec.normal + random_unit_vector();

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero()) {
			scatter_direction = rec.normal;
		}
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;

		return true;
	}

	color get_albedo() const
	{
		return albedo;
	}
	

private:
	color albedo;
};

class metal : public material {
public:
	metal(const color& a, double f) : albedo(a), fuzz(f < 1? f : 1) {}
	
	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz*random_unit_vector());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}


	color get_albedo() const override
	{
		return albedo;
	}


private:
	double fuzz;
	color albedo;
};


class dielectric : public material {
public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
			// Must reflect
			direction = reflect(unit_direction, rec.normal);
		}
		else {
			// Can refract
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}

		scattered = ray(rec.p, direction);
		return true;
	}

	color get_albedo() const override
	{
		return color(0.0, 0.0, 0.0);
	}

private:
	double ir; // Index of refraction

	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approxmimation for reflectance
		double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
	}
};
