#include "renderer.h"

Window::Window(std::string name, int width, int height) {
    extent.width = width;
    extent.height = height;
    this->name = std::move(name);

    if(!glfwInit()) { // Careful with multiple windows!
        throw std::runtime_error("Failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(extent.width, extent.height, this->name.data(), nullptr, nullptr);

    if(!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
}

Window::~Window() {
    if(window) {
        glfwDestroyWindow(window);
    }
}


void Renderer::create_instance() {
    uint32_t extension_count = 0;
    auto extensions = glfwGetRequiredInstanceExtensions(&extension_count);

    vk::ApplicationInfo app_info {"STL Viewer", 1, "NA", 1, VK_API_VERSION_1_2};
    vk::InstanceCreateInfo instance_info {{}, &app_info, 0, nullptr, extension_count, extensions};

    instance = vk::raii::Instance(context, instance_info);
}

Renderer::Renderer() : main_window("STL Viewer", 800, 600) {
    create_instance();

    while(!glfwWindowShouldClose(main_window.window)) {
        glfwPollEvents();
    }
}

Renderer::~Renderer() {
    
    glfwTerminate();
}

