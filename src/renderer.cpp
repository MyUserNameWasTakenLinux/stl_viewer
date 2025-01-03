#include "renderer.h"
#include <iostream>
#include <vector>


bool to_triangle = false;
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *OTHER_FRAGMENT_SHADER = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.7f, 1.0f);\n"
    "}\n\0";


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
    if(glfwGetKey(w, GLFW_KEY_T) == GLFW_PRESS) {
        to_triangle = !to_triangle;
    }
}

Renderer::Renderer() {
    init();
    create_main_window(800, 600, "STL Viewer");

    Shader s("src/shaders/shader.vert", "src/shaders/shader.frag");

    float vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, 0.5, 0.0,
        0.5, -0.5, 0.0
    };

    float other_vertices[] = {
        -0.5, -0.5, 0.0,
        -0.5, 0.5, 0.0,
        0.5, 0.5, 0.0
    };


    std::vector<unsigned int> VAOs;
    std::vector<unsigned int> VBOs;
    VAOs.reserve(2);
    VBOs.reserve(2);
    glGenVertexArrays(2, VAOs.data());
    glGenBuffers(2, VBOs.data());
    
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void* ) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(other_vertices), other_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void* ) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    while (!glfwWindowShouldClose(main_window.handle)) {

        handle_input(main_window.handle);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(!to_triangle) {
            s.use();
            glBindVertexArray(VAOs[0]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        s.use();
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(main_window.handle);
        glfwPollEvents();
    }
}

Renderer::~Renderer() {
    glfwTerminate();
}

