#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "util.h"

struct Window {
    GLFWwindow* handle;
    int width;
    int height;

};

class Renderer {
private:
    Window main_window;

private:
    void init();
    void create_main_window(int width, int height, std::string_view name);
    void handle_input(GLFWwindow* w);
    
public:
    Renderer();

    ~Renderer();

};

#endif