#include "App.h"

App::App()
{
    set_up_glfw();
    set_up_opengl();
    set_up_imgui();

    world.clear();
}

App::~App()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    delete cam;
    delete rayTraceShader;
}

void App::run()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // poll the events
        glfwPollEvents();

        // per-frame logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // ImGui
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            // Create a window called "Render setting" and append into it.
            ImGui::Begin("Render setting");

            static ImGuiComboFlags flags = 0;
            const char* items[] = { "Normal", "Lighting" };
            const char* combo_preview_value = items[shading_mode];  // Pass in the preview value visible before opening the combo (it could be anything)
            if (ImGui::BeginCombo("shading mode", combo_preview_value, flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                {
                    const bool is_selected = (shading_mode == n);
                    if (ImGui::Selectable(items[n], is_selected))
                        shading_mode = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // ImGui::Text("");               // Display some text (you can use a format strings too)
            ImGui::SliderInt("samples per pixel", &samples_per_pixel, 1, 30);     // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderInt("max ray bounce", &maxRayBounce, 1, 30);            // Edit 1 float using a slider from 0.0f to 1.0f
            
            ImGui::Checkbox("anti-alias", &anti_alias);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // rendering commands
        glClearColor(0.3f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glm::mat4 view = cam->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        rayTraceShader->setMat4("view", view);
        rayTraceShader->setMat4("projection", projection);

        rayTraceShader->setInt("maxRayBounce", maxRayBounce);
        rayTraceShader->setInt("SHADING_MODE", shading_mode);
        rayTraceShader->setBool("ANTI_ALIAS", anti_alias);
        rayTraceShader->setInt("samples_per_pixel", (anti_alias) ? samples_per_pixel : 1);
        rayTraceShader->setFloat("screenWidth", SCR_WIDTH);
        
        rayTraceShader->setVec3("cameraPos", cam->Position);

        rayTraceShader->setVec3("viewport_u", cam->viewport_u);
        rayTraceShader->setVec3("viewport_v", cam->viewport_v);
        rayTraceShader->setVec3("viewport_lower_left", cam->viewport_lower_left);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // swap the buffers
        glfwSwapBuffers(window);
    }
}

void App::set_shader_uniform()
{
    rayTraceShader->setFloat("screenHeight", SCR_HEIGHT);

    rayTraceShader->setInt("hittableCount", world.size());
    rayTraceShader->setWorld("sphere", world);
}

void App::addObjectToWorld(shared_ptr<hittable> object)
{
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

    glfwSetFramebufferSizeCallback(window, callback_framebuffer_size);
    // glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, callback_scroll);
}

int App::set_up_imgui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return 0;
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

    
    // rayTraceShader->use();

    glEnable(GL_DEPTH_TEST);
}

void App::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && antiAliasUp) {
        anti_alias = !anti_alias;
        // std::cout << "ANTI_ALIAS: " << anti_alias << std::endl;
        antiAliasUp = false;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        antiAliasUp = true;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && shadeModeUp) {
        shading_mode = ((shading_mode + 1) % 2);
        // shading_mode = (shading_mode == 1) ? 2 : 1;
        // std::cout << "SHADING_MODE: " << shading_mode << std::endl;

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

void App::callback_framebuffer_size(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void App::callback_mouse(GLFWwindow* window, double xposIn, double yposIn)
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
void App::callback_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
    cam->ProcessMouseScroll(static_cast<float>(yoffset));
}

camera* App::cam        = new camera(glm::vec3(0.0f, 0.0f, 3.0f));
float   App::deltaTime  = 1.0f;
float   App::lastFrame  = 1.0f;
float   App::lastX      = SCR_WIDTH / 2.0f;
float   App::lastY      = SCR_HEIGHT / 2.0f;
bool    App::firstMouse = true;

int   App::samples_per_pixel = 10;
bool  App::anti_alias   = true;
int   App::shading_mode = 1;
int   App::maxRayBounce = 15;