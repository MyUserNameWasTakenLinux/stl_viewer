#include "renderer.h"
#include <cstring>


const std::vector<const char*> VALIDATION_LAYERS = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME  
};

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

bool check_validation_layer_support() {
    auto layer_properties = vk::enumerateInstanceLayerProperties();

    for(auto layer : VALIDATION_LAYERS) {
        bool found_layer = false;

        for(auto layer_property : layer_properties) {
            if(strcmp(layer, layer_property.layerName) == 0) {
                found_layer = true;
                break;
            }
        }

        if(found_layer == false) {
            return false;
        }
    }

    return true;
}

bool check_device_extensions(const std::vector<vk::ExtensionProperties>& extension_properties) {
    for(auto extension : DEVICE_EXTENSIONS) {
        bool found_extension = false;

        for(auto extension_property : extension_properties) {
            if(strcmp(extension, extension_property.extensionName) == 0) {
                found_extension = true;
                break;
            }
        }

        if(found_extension == false) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> get_required_extensions() {
    uint32_t extension_count = 0;
    auto glfw_extensions = glfwGetRequiredInstanceExtensions(&extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + extension_count);

    return extensions; 
}

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
    if(ENABLE_VALIDATION_LAYERS && !check_validation_layer_support()) {
        throw std::runtime_error("Validation layers enabled but not available");
    }


    auto extensions = get_required_extensions();
    uint32_t extension_count = static_cast<uint32_t>(extensions.size());

    vk::ApplicationInfo app_info {"STL Viewer", 1, "NA", 1, VK_API_VERSION_1_2};
    vk::InstanceCreateInfo instance_info {{}, &app_info, 0, nullptr, extension_count, extensions.data()};

    if(ENABLE_VALIDATION_LAYERS) {
        instance_info.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        instance_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }

    instance = vk::raii::Instance(context, instance_info);
}

void Renderer::create_surface() {
    VkSurfaceKHR _surface;
    if(glfwCreateWindowSurface(static_cast<VkInstance>(*instance), main_window.window, nullptr, &_surface) != VK_SUCCESS) {
        throw std::runtime_error("Window surface creation failed");
    }
    surface = vk::raii::SurfaceKHR(instance, _surface);
}

void Renderer::select_physical_device() {
    vk::raii::PhysicalDevice physical_device = vk::raii::PhysicalDevices(instance).front();
    auto queue_family_properties = physical_device.getQueueFamilyProperties();

    uint32_t queue_family_index = 0;
    bool queue_family_found = false;
    for(const auto& queue_family_property : queue_family_properties) {
        std::cout << "Queue Count " << queue_family_property.queueCount << "\n";
        if(queue_family_property.queueFlags & vk::QueueFlagBits::eCompute) {
            std::cout << "Supports Compute\n";
        }
        if(queue_family_property.queueFlags & vk::QueueFlagBits::eGraphics) {
            std::cout << "Supports Graphics\n";
        }
        if(queue_family_property.queueFlags & vk::QueueFlagBits::eTransfer) {
            std::cout << "Supports Transfer\n";
        }

        auto surface_support = physical_device.getSurfaceSupportKHR(queue_family_index, surface);
        if(queue_family_property.queueFlags & vk::QueueFlagBits::eGraphics && surface_support == vk::True) {
            queue_family_found = true;
            break;
        }
        
        ++queue_family_index;
    }

    if(!queue_family_found) {
        throw std::runtime_error("Could not find a graphics queue");
    }

    auto extension_properties = physical_device.enumerateDeviceExtensionProperties();
    if(!check_device_extensions(extension_properties)) {
        throw std::runtime_error("Required device extensions unavailable");
    }

    float queue_priority = 0.0f;
    vk::DeviceQueueCreateInfo device_queue_info({}, queue_family_index, 1, &queue_priority);
    vk::DeviceCreateInfo device_info({}, device_queue_info);
    device_info.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    device_info.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

    device = vk::raii::Device(physical_device, device_info);

}

Renderer::Renderer() : main_window("STL Viewer", 800, 600) {
    create_instance();
    create_surface();
    select_physical_device();

    while(!glfwWindowShouldClose(main_window.window)) {
        glfwPollEvents();
    }
}

Renderer::~Renderer() {
    
    glfwTerminate();
}

