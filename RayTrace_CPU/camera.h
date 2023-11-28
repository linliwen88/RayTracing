#pragma once
#include <fstream>

#include "color.h"
#include "common.h"
#include "material.h"
#include "hittable.h"
#include "hittable_list.h"

class camera_mul {
public:
    double  aspect_ratio = 16.0 / 9.0; // Ratio of image width over height
    int     image_width = 400;         // Rendered image width in pixel count
    int     image_height;              // Rendered image height
    int     samples_per_pixel = 10;    // Count of random samples for each pixel
    int     max_ray_bounce = 10;       // Maximum number of ray bounces into scene

    double  vfov     = 90;                 // Vertical view angle (degrees)
    point3  lookfrom = point3(0, 0, -1);   // Point camera is looking from
    point3  lookat   = point3(0, 0, 0);    // Point camera is looking at
    vec3    up       = vec3(0, 1, 0);      // Camera-relative up direction

    static int remaining_scanline;

    // Render image setup
    void initialize()
    {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        render_result.clear();
        render_result.resize(image_height, vector<color>(image_width));

        // Camera settings and determine viewport dimensions
        camera_center = lookfrom;
        double focal_length = (lookfrom - lookat).length();

        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2.0);
        double viewport_height = 2.0 * h * focal_length;
        double viewport_width = viewport_height * (static_cast<double>(image_width) / static_cast<double>(image_height));

        // Calculate u,v,w unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(up, w));
        v = cross(w, u);

        // Calculate vectors across the horizontal and down the vertical viewport edges
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // Calculate deltas from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;


        // Calculate the position of the upper left pixel
        point3 viewport_upper_left = camera_center - (focal_length * w) - (viewport_u / 2) - (viewport_v / 2);
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    void render(const hittable_list& world, const int start, const int end)
    {
        for (int j = start; j < end; j++)
        {
            // std::clog << "\rScanlines remaining: " << end - j << " / " << end - start << std::flush;
            for (int i = 0; i < image_width; i++)
            {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_ray_bounce, world);
                }
                // write_color(RenderOutput, pixel_color, samples_per_pixel);
                render_result[j][i] = pixel_color;

            }
        }
        // std::clog << "\rDone.                       \n";
    }

    void output()
    {
        // Write to output file
        std::ofstream RenderOutput;
        RenderOutput.open("image.ppm");
        RenderOutput << "P3\n" << image_width << " " << image_height << "\n255\n";
        write_colors(RenderOutput, render_result, samples_per_pixel);
        RenderOutput.close();
    }

private:

    point3 camera_center;   // Camera center

    point3 pixel00_loc;     // Location of pixel 0, 0
    vec3 pixel_delta_u;     // Offset to pixel to the right
    vec3 pixel_delta_v;     // Offset to pixel below
    vec3 u, v, w;           // Camera basis frame vectors

    vector<vector<color>> render_result;

    color ray_color(const ray& r, int ray_bounce, const hittable_list& world) const
    {
        hit_record rec;
        if (ray_bounce <= 0)
        {
            return color(0, 0, 0);
        }

        if (world.hit(r, interval(0.002, infinity), rec))
        {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
            {
                return attenuation * ray_color(scattered, ray_bounce - 1, world);
            }
            else
            {
                return rec.mat->get_albedo();
            }
        }

        vec3 unit_direction = unit_vector(r.direction()); // [-1, 1]
        float a = 0.7 * (unit_direction.y() + 1.0); // [0, 1]

        color blue = color(0.5, 0.7, 1.0);
        color white = color(1.0, 1.0, 1.0);

        return (1.0f - a) * white + a * blue;
    }

    // Get a randomly sampled camera ray for the pixel at location i, j
    ray get_ray(int i, int j) const
    {
        point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        point3 pixel_sample = pixel_center + pixel_sample_square();
        point3 pixel_sample = pixel_center + pixel_sample_square();

        point3 ray_origin = camera_center;
        vec3 ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    // Returns a random point in the square surrounding a pixel at the origin
    vec3 pixel_sample_square() const
    {
        double px = -0.5 + random_double();
        double py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};
