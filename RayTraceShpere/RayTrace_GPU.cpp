// RayTrace_GPU.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <glm/glm.hpp>

#include <memory>

#include "App.h"
#include "sphere.h"

int main()
{
    struct {
        int metal      = 1;
        int lambertian = 2;
        int dielectric = 3;
    } material;

    static App* myRayTracer = new App();

    // add hittable spheres into world
    auto sphere_1 = std::make_shared<sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, material.metal);
    auto sphere_2 = std::make_shared<sphere>(glm::vec3(0.75f, -0.25f, -1.0f), 0.25f, material.lambertian);
    auto ground   = std::make_shared<sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, material.dielectric);

    myRayTracer->addObjectToWorld(sphere_1);
    myRayTracer->addObjectToWorld(sphere_2);
    myRayTracer->addObjectToWorld(ground);

    myRayTracer->set_shader_uniform();

    // start render loop
    myRayTracer->run();

    delete myRayTracer;
    return 0;
}