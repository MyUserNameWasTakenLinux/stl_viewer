#include "util.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(std::filesystem::path vs_path, std::filesystem::path fs_path) {
    if(!std::filesystem::exists(vs_path)) {
        throw std::runtime_error("Vertex shader file does not exist");
    }
    if(!std::filesystem::exists(fs_path)) {
        throw std::runtime_error("Fragment shader file does not exist");
    }
    
    std::string vs_code;
    std::string fs_code;
    std::ifstream vs_file(vs_path);
    std::ifstream fs_file(fs_path);

    if(!vs_file.is_open()) {
        throw std::runtime_error("Failed to open vertex shader file");
    }
    if(!fs_file.is_open()) {
        throw std::runtime_error("Failed to open fragment shader file");
    }

    std::stringstream vs_stream, fs_stream;
    vs_stream << vs_file.rdbuf();
    fs_stream << fs_file.rdbuf();

    vs_file.close();
    fs_file.close();

    vs_code = vs_stream.str();
    fs_code = fs_stream.str();

    const char* cstr_vs = vs_code.c_str();
    const char* cstr_fs = fs_code.c_str();

    unsigned int vs_id, fs_id;
    
    vs_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs_id, 1, &cstr_vs, NULL);
    glCompileShader(vs_id);
    check_compile_error(vs_id, "Vertex");

    fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_id, 1, &cstr_fs, NULL);
    glCompileShader(fs_id);
    check_compile_error(fs_id, "Fragment");

    id = glCreateProgram();
    glAttachShader(id, vs_id);
    glAttachShader(id, fs_id);
    glLinkProgram(id);
    check_compile_error(id, "Program");

    glDeleteShader(vs_id);
    glDeleteShader(fs_id);
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::check_compile_error(GLuint id, std::string type) {
    int success;
    char info_log[512];

    if(type != "Program") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(id, sizeof(info_log), NULL, info_log);
            std::cout << "Shader compilation failed" << info_log << std::endl;
        }
    } else {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(id, sizeof(info_log), NULL, info_log);
            std::cout << "Program linkage failed" << info_log << std::endl;
        }
    }
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {

}

Mesh::~Mesh() {

}

void Mesh::draw(Shader& s) {

}
