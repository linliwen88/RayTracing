// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <memory>

#include "Utilities.h"

#include "App.h"
#include "sphere.h"


int main()
{
    static App* myRayTracer = new App();

    // add hittable spheres into world
    auto sphere_1 = std::make_shared<sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);
    auto sphere_2 = std::make_shared<sphere>(glm::vec3(0.75f, -0.25f, -1.0f), 0.25f);
    auto ground = std::make_shared<sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f);

    myRayTracer->addObjectToWorld(sphere_1);
    myRayTracer->addObjectToWorld(sphere_2);
    myRayTracer->addObjectToWorld(ground);

    myRayTracer->run();

    return 0;
}