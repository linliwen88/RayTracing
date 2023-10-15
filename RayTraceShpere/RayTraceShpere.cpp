// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <vector>

#include "RenderAttributes.h"
#include "Utilities.h"
#include "Shader.h"
#include "Camera.h"
#include "Sphere.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


// traingle vertices in clip space
float vertices[] = {
    -1.0f, -1.0f, 0.0f, // bottom left
    1.0f, -1.0f, 0.0f, // bottom right
    -1.0f, 1.0f, 0.0f,// top left

    -1.0f, 1.0f, 0.0f,// top left
    1.0f, -1.0f, 0.0f, // bottom right
    1.0f, 1.0f, 0.0f // top right
};

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// movement speed
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f; // time of last frame

// lighting
glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);

// RAY TRACING WORLD
std::vector<Sphere* > world;

int main()
{
    // Intialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    GLFWwindow* window = glfwCreateWindow((int)SCR_WIDTH, (int)SCR_HEIGHT, "Ray Trace Sphere", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    std::cout << "Hello World!\n";

    // bind vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex buffer objectsa
    unsigned int VBO;
    glGenBuffers(1, &VBO); // generating buffer ID
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // shaders
    Shader rayTraceShader("shaders/vshader.glsl", "shaders/fshader.glsl");
    rayTraceShader.use();

    // add hittable spheres into ray tracing world
    Sphere sphere_1(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);
    Sphere sphere_2(glm::vec3(0.75f, -0.25f, -1.0f), 0.25f);
    Sphere ground(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f);
    world.push_back(&sphere_1);
    world.push_back(&sphere_2);
    world.push_back(&ground);


    // render loop
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {

        // per-frame logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // rendering commands
        glClearColor(0.3f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        rayTraceShader.setMat4("view", view);
        rayTraceShader.setMat4("projection", projection);


        rayTraceShader.setInt("SHADING_MODE", SHADING_MODE);
        rayTraceShader.setBool("ANTI_ALIAS", ANTI_ALIAS);
        rayTraceShader.setInt("samples_per_pixel", (ANTI_ALIAS)? samples_per_pixel : 1);
        rayTraceShader.setFloat("screenWidth", SCR_WIDTH);
        rayTraceShader.setFloat("screenHeight", SCR_HEIGHT);

        rayTraceShader.setVec3("cameraPos", camera.Position);
        rayTraceShader.setVec3("horizontal", camera.horizontal);
        rayTraceShader.setVec3("vertical", camera.vertical);
        rayTraceShader.setVec3("lowerLeftCorner", camera.lowerLeftCorner);

        rayTraceShader.setInt("hittableCount", world.size());
        rayTraceShader.setWorld("sphere", world);
        rayTraceShader.use();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glBindVertexArray(0);


        // check all events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && antiAliasUp) {
        ANTI_ALIAS = !ANTI_ALIAS;
        std::cout << "ANTI_ALIAS: " << ANTI_ALIAS << std::endl;
        antiAliasUp = false;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        antiAliasUp = true;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && shadeModeUp) {
        SHADING_MODE = (SHADING_MODE + 1) % 2;
        std::cout << "SHADING_MODE: " << SHADING_MODE << std::endl;
        shadeModeUp = false;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
        shadeModeUp = true;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}