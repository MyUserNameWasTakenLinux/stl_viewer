#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>
#include <iostream>
#include <vector>

struct Window {
    Window(std::string name, int width, int height);
    ~Window();

    GLFWwindow* window;
    std::string name;
    vk::Extent2D extent;
};

class Renderer {
private:

Window main_window;
vk::raii::Context context;
vk::raii::Instance instance {nullptr};

private:

void create_instance();

public:

Renderer();

~Renderer();

};


#endif