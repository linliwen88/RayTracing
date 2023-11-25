#include "App.h"

App::App()
{
    this->set_up_glfw();
    this->set_up_opengl();
    // this->set_up_camera();

    world.clear();
}

App::~App()
{
    glfwTerminate();
    delete cam;
    delete rayTraceShader;
}

void App::run()
{
    // render loop
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

        glm::mat4 view = cam->GetViewMatrix();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        rayTraceShader->setMat4("view", view);
        rayTraceShader->setMat4("projection", projection);


        rayTraceShader->setInt("SHADING_MODE", SHADING_MODE);
        rayTraceShader->setBool("ANTI_ALIAS", ANTI_ALIAS);
        rayTraceShader->setInt("samples_per_pixel", (ANTI_ALIAS) ? samples_per_pixel : 1);
        rayTraceShader->setFloat("screenWidth", SCR_WIDTH);
        rayTraceShader->setFloat("screenHeight", SCR_HEIGHT);

        rayTraceShader->setVec3("cameraPos", cam->Position);
        rayTraceShader->setVec3("horizontal", cam->horizontal);
        rayTraceShader->setVec3("vertical", cam->vertical);
        rayTraceShader->setVec3("lowerLeftCorner", cam->lowerLeftCorner);

        rayTraceShader->setInt("hittableCount", world.size());
        rayTraceShader->setWorld("sphere", world);
        rayTraceShader->use();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);

        // check all events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void App::addObjectToWorld(shared_ptr<hittable> object)
{
    // std::cout << "Adding sphere radius: " << std::static_pointer_cast<sphere>(object)->Radius << std::endl;
    world.push_back(object);
}

int App::set_up_glfw()
{
    // Intialize and configure glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    window = glfwCreateWindow((int)SCR_WIDTH, (int)SCR_HEIGHT, "Ray Trace GPU", NULL, NULL);
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
}

void App::set_up_opengl()
{
    // traingle vertices in clip space
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, // bottom left
        1.0f, -1.0f, 0.0f, // bottom right
        -1.0f, 1.0f, 0.0f,// top left

        -1.0f, 1.0f, 0.0f,// top left
        1.0f, -1.0f, 0.0f, // bottom right
        1.0f, 1.0f, 0.0f // top right
    };

    // bind vertex array object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex buffer objectsa
    glGenBuffers(1, &VBO); // generating buffer ID
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // shaders
    rayTraceShader = new shader("shaders/vshader.glsl", "shaders/fshader.glsl");
    rayTraceShader->use();

    glEnable(GL_DEPTH_TEST);
}

void App::processInput(GLFWwindow* window)
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
        cam->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam->ProcessKeyboard(RIGHT, deltaTime);
}

void App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void App::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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

    cam->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam->ProcessMouseScroll(static_cast<float>(yoffset));
}

float   App::deltaTime  = 1.0f;
float   App::lastFrame  = 1.0f;
float   App::lastX      = SCR_WIDTH / 2.0f;
float   App::lastY      = SCR_HEIGHT / 2.0f;
bool    App::firstMouse = true;
camera* App::cam = new camera(glm::vec3(0.0f, 0.0f, 3.0f));