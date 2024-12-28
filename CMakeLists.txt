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

# Shader compilation
find_program(GLSLC glslc)
if(NOT GLSLC)
    message(FATAL_ERROR "glslc not found! Make sure Vulkan SDK is installed and glslc is in PATH.")
endif()

set(shader_path "${CMAKE_SOURCE_DIR}/src/shaders/")
file(GLOB shaders "${shader_path}*.vert" "${shader_path}*.frag")

foreach(shader ${shaders})
    get_filename_component(shader_name ${shader} NAME)
    set(output_spv "${CMAKE_BINARY_DIR}/${shader_name}.spv")
    add_custom_command(
        OUTPUT ${output_spv}
        COMMAND ${GLSLC} ${shader} -o ${output_spv}
        DEPENDS ${shader}
        COMMENT "Compiling ${shader} to ${output_spv}"
    )
    list(APPEND SPIRV_SHADERS ${output_spv})
endforeach()

# Add custom target for shaders
add_custom_target(Shaders ALL DEPENDS ${SPIRV_SHADERS})

# Ensure shaders are built before the main project
add_dependencies(${PROJECT_NAME} Shaders)

# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glm::glm glfw)

# Add compile options if needed (optional)
target_compile_options(${PROJECT_NAME} PRIVATE -g)
