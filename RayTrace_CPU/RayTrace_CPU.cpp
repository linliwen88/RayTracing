// RayTrace_CPU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <chrono>

#include "common.h"
#include "sphere.h"
// #include "cube.h"
#include "camera.h"
#include "hittable_list.h"

using namespace std::chrono;

int main()
{
    // Put objects in `world` to render
    static hittable_list world;

    //// Create materials
    //auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    //auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    //auto material_left   = make_shared<dielectric>(1.5);
    //auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.1);


    //world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    //world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    //world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    //world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.49, material_left));
    //world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Setup camera 
    camera_mul cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 50;
    cam.max_ray_bounce = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.up = vec3(0, 1, 0);

    cam.samples_per_pixel = 50;
    cam.max_ray_bounce = 10;

    cam.initialize();
    int scanline_per_thread = cam.image_height / 8;

    auto start = high_resolution_clock::now();
    std::thread work_1(&camera_mul::render, &cam, world,                       0,     scanline_per_thread);
    std::thread work_2(&camera_mul::render, &cam, world,     scanline_per_thread, 2 * scanline_per_thread);
    std::thread work_3(&camera_mul::render, &cam, world, 2 * scanline_per_thread, 3 * scanline_per_thread);
    std::thread work_4(&camera_mul::render, &cam, world, 3 * scanline_per_thread, 4 * scanline_per_thread);
    std::thread work_5(&camera_mul::render, &cam, world, 4 * scanline_per_thread, 5 * scanline_per_thread);
    std::thread work_6(&camera_mul::render, &cam, world, 5 * scanline_per_thread, 6 * scanline_per_thread);
    std::thread work_7(&camera_mul::render, &cam, world, 6 * scanline_per_thread, 7 * scanline_per_thread);
    std::thread work_8(&camera_mul::render, &cam, world, 7 * scanline_per_thread,        cam.image_height);
    work_1.join();
    work_2.join();
    work_3.join();
    work_4.join();
    work_5.join();
    work_6.join();
    work_7.join();
    work_8.join();

    auto stop = high_resolution_clock::now();

    cam.output();

    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "Time taken by multithreading function: " << (double)duration.count() / 1000 << " seconds" << std:: endl;


    return 0;    
}
