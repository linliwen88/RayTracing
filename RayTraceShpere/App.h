#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector> 
#include <memory>
#include <iostream>

#include "RenderSetting.h"
#include "stb_image.h"
#include "camera.h"
#include "shader.h"
#include "hittable.h"

using std::vector;
using std::shared_ptr;

class App {
public:
	App();
	~App();

	void run();
	void set_shader_uniform();
	void addObjectToWorld(shared_ptr<hittable> object);

	static camera* cam;
	GLFWwindow* window;


private:
	int set_up_glfw();
	int set_up_imgui();
	void set_up_opengl();

	// ImGui
	bool show_demo_window = false;
	bool show_another_window = false;
	const char* glsl_version = "#version 330";

	// OpenGL buffers
	unsigned int VAO;
	unsigned int VBO;
	shader* rayTraceShader;
	vector<shared_ptr<hittable>> world;

	// render atibutes
	static bool anti_alias;
	static int shading_mode;

	// movement speed
	static float deltaTime; // time between current frame and last frame
	static float lastFrame; // time of last frame

	// inputs and callback functions for input
	static float lastX;
	static float lastY;
	static bool firstMouse;

	static void processInput(GLFWwindow* window);
	static void callback_framebuffer_size(GLFWwindow* window, int width, int height);
	static void callback_mouse(GLFWwindow* window, double xpos, double ypos);
	static void callback_scroll(GLFWwindow* window, double xoffset, double yoffset);
};
