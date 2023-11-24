#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

#include "RenderSetting.h"
#include "stb_image.h"
#include "camera.h"
#include "Hittable.h"


class App {
public:
	App();
	~App();

	static camera* cam;
	GLFWwindow* window;


private:
	int set_up_glfw();
	void set_up_opengl();
	void set_up_camera();
	inline void add_hittable(Hittable* hittable) { world.push_back(hittable); }

	std::vector<Hittable*> world;

	// inputs and callback functions for input
	static float lastX;
	static float lastY;
	static bool firstMouse;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};
