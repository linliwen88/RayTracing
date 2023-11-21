#pragma once
#include <iostream>
#include <vector>
#include "vec3.h"

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
	return sqrt(linear_component);
}

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	// Devide the color by the number of samples
	double scale = 1.0 / static_cast<double>(samples_per_pixel);

	r *= scale;
	g *= scale;
	b *= scale;

	// Apply the linea to gamma transform
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	// Write the translated [0, 255] value of each color component
	static const interval intensity(0.000, 0.999);

	out << static_cast<int>(256 * intensity.clamp(r)) << ' '
		<< static_cast<int>(256 * intensity.clamp(g)) << ' '
		<< static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

void write_colors(std::ostream& out, std::vector<std::vector<color>>& pixel_color, int samples_per_pixel)
{
	int rows = pixel_color.size();
	int cols = pixel_color[0].size();

	for (int R = 0; R < rows; R++)
	{
		for (int C = 0; C < cols; C++)
		{
			double r = pixel_color[R][C].x();
			double g = pixel_color[R][C].y();
			double b = pixel_color[R][C].z();

			// Devide the color by the number of samples
			double scale = 1.0 / static_cast<double>(samples_per_pixel);

			r *= scale;
			g *= scale;
			b *= scale;

			// Apply the linea to gamma transform
			r = linear_to_gamma(r);
			g = linear_to_gamma(g);
			b = linear_to_gamma(b);

			// Write the translated [0, 255] value of each color component
			static const interval intensity(0.000, 0.999);

			out << static_cast<int>(256 * intensity.clamp(r)) << ' '
				<< static_cast<int>(256 * intensity.clamp(g)) << ' '
				<< static_cast<int>(256 * intensity.clamp(b)) << '\n';
		}
	}
	
}