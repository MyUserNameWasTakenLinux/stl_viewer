#ifndef RENDERER_H
#define RENDERER_H

#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <optional>

struct Window {
    Window(std::string name, int width, int height);
    ~Window();

    GLFWwindow* window;
    std::string name;
    vk::Extent2D extent;
};

struct SwapChainData {
    vk::raii::SwapchainKHR swapchain {nullptr};
    vk::Format color_format;
    std::vector<vk::Image> images;
    std::vector<vk::raii::ImageView> image_views;
    vk::Extent2D swapchain_extent;

};

class Renderer {
private:

SwapChainData swapchain_data; // Should destruct before main_window to avoid segfaulting
Window main_window;
vk::raii::Context context;
vk::raii::Instance instance {nullptr};
vk::raii::SurfaceKHR surface {nullptr};
vk::raii::Device device {nullptr};
std::optional<uint32_t> queue_family_index; // Assuming single queue family for graphics and present

private:

void create_instance();

void create_surface();

void select_physical_device();

void create_swapchain();

public:

Renderer();

~Renderer();

};


#endif