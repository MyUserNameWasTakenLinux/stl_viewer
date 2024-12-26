#ifndef RENDERER_H
#define RENDERER_H

#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
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
vk::raii::SurfaceKHR surface {nullptr};
vk::raii::Device device {nullptr};

private:

void create_instance();

void create_surface();

void select_physical_device();

public:

Renderer();

~Renderer();

};


#endif