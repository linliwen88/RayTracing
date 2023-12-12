// RayTrace_GPU.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <glm/glm.hpp>

#include <memory>

#include "App.h"
#include "material.h"
#include "sphere.h"
#include "Helper.h"

int main()
{

    // viewport size
    const float ASPECT_RATIO = 16.0 / 9.0;
    const int SCR_HEIGHT = 600;

    static App* myRayTracer = new App(SCR_HEIGHT, ASPECT_RATIO);

    const int metal = 0;
    const int lambertian = 1;
    const int dielectric = 2;

    //// create materials
    //auto material_center = std::make_shared<material>(lambertian, glm::vec3(0.1f, 0.2f, 0.5f));
    //auto material_right  = std::make_shared<material>(metal, glm::vec3(0.8f, 0.6f, 0.2f));
    //auto material_left   = std::make_shared<material>(metal, glm::vec3(0.5f, 0.2f, 0.1f));
    //auto material_ground = std::make_shared<material>(lambertian, glm::vec3(0.8f, 0.8f, 0.0f));

    //// add hittable spheres into world
    //auto sphere_center = std::make_shared<sphere>(glm::vec3(0.0f, 0.0f, -1.0f)   , 0.5f  , material_center);
    //auto sphere_right  = std::make_shared<sphere>(glm::vec3(1.0f, 0.0f, -1.0f)   , 0.5f  , material_right);
    //auto sphere_left   = std::make_shared<sphere>(glm::vec3(-1.0f, 0.0f, -1.0f)  , 0.5f  , material_left);
    //auto ground        = std::make_shared<sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, material_ground);

    //myRayTracer->addObjectToWorld(sphere_center);
    //myRayTracer->addObjectToWorld(sphere_right);
    //myRayTracer->addObjectToWorld(sphere_left);
    //myRayTracer->addObjectToWorld(ground);

    auto material_ground = std::make_shared<material>(lambertian, glm::vec3(0.5f, 0.5f, 0.5f));
    auto ground = std::make_shared<sphere>(glm::vec3(0.0f, -1000.f,  0.f), 1000.0f, material_ground);
    myRayTracer->addObjectToWorld(ground);


    for (int a = -5; a < 5; a++)
    {
        for (int b = -5; b < 5; b++)
        {
            auto choose_mat = random_float();
            glm::vec3 center(a + 0.9f * random_float(), 0.2, b + 0.9 + random_float());

            if ((center - glm::vec3(4.f, 0.2f, 0.f)).length() > 0.9f)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8f)
                {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    sphere_material = std::make_shared<material>(lambertian, albedo);
                    myRayTracer->addObjectToWorld(std::make_shared<sphere>(center, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95f)
                {
                    // metal
                    auto albedo = vec3::random(0.5, 1);
                    // auto fuzz = random_float(0, 0.5);
                    sphere_material = std::make_shared<material>(metal, albedo);
                    myRayTracer->addObjectToWorld(std::make_shared<sphere>(center, 0.2f, sphere_material));
                }
                else
                {
                    // glass
                    auto albedo = vec3::random(0.5, 1);
                    sphere_material = std::make_shared<material>(dielectric, albedo);
                    myRayTracer->addObjectToWorld(std::make_shared<sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    myRayTracer->set_shader_uniform();

    // start render loop
    myRayTracer->run();

    delete myRayTracer;
    return 0;
}