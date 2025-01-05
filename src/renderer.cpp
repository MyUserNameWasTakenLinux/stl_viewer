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

        std::vector<Vertex> vertices = {
        // Front face
        {glm::vec3(-0.5, -0.5,  0.5), glm::vec3(0.5, 0.2, 0.8)}, // Bottom-left
        {glm::vec3( 0.5, -0.5,  0.5), glm::vec3(0.5, 0.2, 0.8)}, // Bottom-right
        {glm::vec3( 0.5,  0.5,  0.5), glm::vec3(0.5, 0.2, 0.8)}, // Top-right
        {glm::vec3(-0.5,  0.5,  0.5), glm::vec3(0.5, 0.2, 0.8)}, // Top-left

        // Back face
        {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.2, 0.8, 0.2)}, // Bottom-left
        {glm::vec3( 0.5, -0.5, -0.5), glm::vec3(0.2, 0.8, 0.2)}, // Bottom-right
        {glm::vec3( 0.5,  0.5, -0.5), glm::vec3(0.2, 0.8, 0.2)}, // Top-right
        {glm::vec3(-0.5,  0.5, -0.5), glm::vec3(0.2, 0.8, 0.2)}, // Top-left

        // Left face
        {glm::vec3(-0.5, -0.5,  0.5), glm::vec3(0.8, 0.5, 0.2)}, // Front-bottom
        {glm::vec3(-0.5,  0.5,  0.5), glm::vec3(0.8, 0.5, 0.2)}, // Front-top
        {glm::vec3(-0.5,  0.5, -0.5), glm::vec3(0.8, 0.5, 0.2)}, // Back-top
        {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.8, 0.5, 0.2)}, // Back-bottom

        // Right face
        {glm::vec3( 0.5, -0.5,  0.5), glm::vec3(0.2, 0.5, 0.8)}, // Front-bottom
        {glm::vec3( 0.5,  0.5,  0.5), glm::vec3(0.2, 0.5, 0.8)}, // Front-top
        {glm::vec3( 0.5,  0.5, -0.5), glm::vec3(0.2, 0.5, 0.8)}, // Back-top
        {glm::vec3( 0.5, -0.5, -0.5), glm::vec3(0.2, 0.5, 0.8)}, // Back-bottom

        // Top face
        {glm::vec3(-0.5,  0.5,  0.5), glm::vec3(0.8, 0.8, 0.2)}, // Front-left
        {glm::vec3( 0.5,  0.5,  0.5), glm::vec3(0.8, 0.8, 0.2)}, // Front-right
        {glm::vec3( 0.5,  0.5, -0.5), glm::vec3(0.8, 0.8, 0.2)}, // Back-right
        {glm::vec3(-0.5,  0.5, -0.5), glm::vec3(0.8, 0.8, 0.2)}, // Back-left

        // Bottom face
        {glm::vec3(-0.5, -0.5,  0.5), glm::vec3(0.2, 0.8, 0.8)}, // Front-left
        {glm::vec3( 0.5, -0.5,  0.5), glm::vec3(0.2, 0.8, 0.8)}, // Front-right
        {glm::vec3( 0.5, -0.5, -0.5), glm::vec3(0.2, 0.8, 0.8)}, // Back-right
        {glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.2, 0.8, 0.8)}  // Back-left
    };
    std::vector<Vertex> other_vertices = {
        {glm::vec3(-0.5, -0.5, 0.0), glm::vec3(0.5, 0.2, 0.8)},
        {glm::vec3(-0.5, 0.5, 0.0), glm::vec3(0.5, 0.2, 0.8)},
        {glm::vec3(0.5, 0.5, 0.0), glm::vec3(0.5, 0.2, 0.8)}
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2,  0, 2, 3,
        // Back face
        4, 5, 6,  4, 6, 7,
        // Left face
        8, 9, 10, 8, 10, 11,
        // Right face
        12, 13, 14, 12, 14, 15,
        // Top face
        16, 17, 18, 16, 18, 19,
        // Bottom face
        20, 21, 22, 20, 22, 23
    };

    auto triangle = Mesh(vertices, indices);
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
        view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 200.0f); // Careful with aspect ratio
        s.set_mat4("model", model);
        s.set_mat4("view", view);
        s.set_mat4("projection", projection);
        triangle.draw();

        glfwSwapBuffers(main_window.handle);
        glfwPollEvents();
    }
}

Renderer::~Renderer() {
    glfwTerminate();
}

