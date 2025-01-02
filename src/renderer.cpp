#include "renderer.h"
#include <iostream>

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
        glfwWindowShouldClose(main_window.handle);
    }
}

Renderer::Renderer() {
    init();
    create_main_window(800, 600, "STL Viewer");

    while (!glfwWindowShouldClose(main_window.handle)) {

        handle_input(main_window.handle);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(main_window.handle);
        glfwPollEvents();
    }
}

Renderer::~Renderer() {
    glfwTerminate();
}

