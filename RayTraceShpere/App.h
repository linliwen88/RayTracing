#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <memory>
#include <iostream>

#include "RenderSetting.h"
#include "stb_image.h"
#include "camera.h"
#include "shader.h"
#include "hittable.h"
#include "sphere.h"

using std::vector;
using std::shared_ptr;

class App {
public:
	App();
	~App();

	void run();
	void addObjectToWorld(shared_ptr<hittable> object);

	static camera* cam;
	GLFWwindow* window;


private:
	int set_up_glfw();
	void set_up_opengl();
	// void set_up_camera();

	unsigned int VAO;
	unsigned int VBO;
	shader* rayTraceShader;
	vector<shared_ptr<hittable>> world;

	// movement speed
	static float deltaTime; // time between current frame and last frame
	static float lastFrame; // time of last frame

	// inputs and callback functions for input
	static float lastX;
	static float lastY;
	static bool firstMouse;

	static void processInput(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};
