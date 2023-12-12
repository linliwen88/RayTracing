// RayTrace_GPU.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <glm/glm.hpp>

#include <memory>

#include "App.h"
#include "material.h"
#include "sphere.h"

int main()
{

    // viewport size
    const float ASPECT_RATIO = 16.0 / 9.0;
    const int SCR_HEIGHT = 600;

    static App* myRayTracer = new App(SCR_HEIGHT, ASPECT_RATIO);

    const int metal = 0;
    const int lambertian = 1;
    const int dielectric = 2;


    // create materials
    auto material_center = std::make_shared<material>(lambertian, glm::vec3(0.1f, 0.2f, 0.5f));
    auto material_right  = std::make_shared<material>(metal, glm::vec3(0.8f, 0.6f, 0.2f));
    auto material_left   = std::make_shared<material>(metal, glm::vec3(0.5f, 0.2f, 0.1f));
    auto material_ground = std::make_shared<material>(lambertian, glm::vec3(0.8f, 0.8f, 0.0f));

    // add hittable spheres into world
    auto sphere_center = std::make_shared<sphere>(glm::vec3(0.0f, 0.0f, -1.0f)   , 0.5f  , material_center);
    auto sphere_right  = std::make_shared<sphere>(glm::vec3(1.0f, 0.0f, -1.0f)   , 0.5f  , material_right);
    auto sphere_left   = std::make_shared<sphere>(glm::vec3(-1.0f, 0.0f, -1.0f)  , 0.5f  , material_left);
    auto ground        = std::make_shared<sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, material_ground);

    myRayTracer->addObjectToWorld(sphere_center);
    myRayTracer->addObjectToWorld(sphere_right);
    myRayTracer->addObjectToWorld(sphere_left);
    myRayTracer->addObjectToWorld(ground);

    myRayTracer->set_shader_uniform();

    // start render loop
    myRayTracer->run();

    delete myRayTracer;
    return 0;
}