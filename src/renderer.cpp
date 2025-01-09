#include "renderer.h"
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Renderer::init() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Renderer::create_main_window(int width, int height, std::string_view name) {
    main_window.handle = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
    main_window.width = width;
    main_window.height = height;
    glfwMakeContextCurrent(main_window.handle);
    glfwSetFramebufferSizeCallback(main_window.handle, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

void Renderer::handle_input(GLFWwindow* w) {
    if(glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(main_window.handle, true);
    }
}

Renderer::Renderer() {
    init();
    create_main_window(800, 600, "STL Viewer");

    Shader s("src/shaders/shader.vert", "src/shaders/shader.frag");

    auto cube = Mesh("TestCube.stl");

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(main_window.handle)) {

        handle_input(main_window.handle);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        s.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0, 0.0, -60.0));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 200.0f); // Careful with aspect ratio
        s.set_mat4("model", model);
        s.set_mat4("view", view);
        s.set_mat4("projection", projection);
        cube.draw();

        glfwSwapBuffers(main_window.handle);
        glfwPollEvents();
    }
}

Renderer::~Renderer() {
    glfwTerminate();
}

