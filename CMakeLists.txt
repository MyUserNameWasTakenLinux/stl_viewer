# Minimum CMake version
cmake_minimum_required(VERSION 3.14)

# Project Name and Version
project(STL_VIEWER VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source directory
add_executable(${PROJECT_NAME} src/main.cpp src/renderer.cpp)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Find GLM
find_package(glm CONFIG REQUIRED)

# Find GLFW
include_directories("/usr/include")
target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw 
    vulkan 
    dl 
    pthread 
    X11 
    Xxf86vm 
    Xrandr 
    Xi
)


# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glm::glm glfw)


# Add compile options if needed (optional)
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

