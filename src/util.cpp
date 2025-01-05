#include "util.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <cstring>

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

void Shader::set_bool(std::string_view name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.data()), (int) value);
}

void Shader::set_int(std::string_view name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.data()), value);
}

void Shader::set_float(std::string_view name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.data()), value);
}

void Shader::set_vec2(std::string_view name, glm::vec2 value) const {
    glUniform2fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
}

void Shader::set_vec2(std::string_view name, float x, float y) const {
    glUniform2f(glGetUniformLocation(id, name.data()), x, y);
}

void Shader::set_vec3(std::string_view name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
}

void Shader::set_vec3(std::string_view name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name.data()), x, y, z);
}

void Shader::set_vec4(std::string_view name, glm::vec4 value) const {
    glUniform4fv(glGetUniformLocation(id, name.data()), 1, &value[0]);
}

void Shader::set_vec4(std::string_view name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(id, name.data()), x, y, z, w);
}

void Shader::set_mat2(std::string_view name, glm::mat2 value) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &value[0][0]);
}

void Shader::set_mat3(std::string_view name, glm::mat3 value) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &value[0][0]);
}

void Shader::set_mat4(std::string_view name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, &value[0][0]);
}

void Shader::check_compile_error(GLuint id, std::string type) const {
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

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : vertices(vertices), indices(indices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, color));

    glBindVertexArray(0);
}

Mesh::Mesh(std::filesystem::path stl_path) {
    if (!std::filesystem::exists(stl_path)) {
        throw std::runtime_error("STL file not found");
    }

    std::ifstream file(stl_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Error: Could not open the file");
    }

    file.seekg(80, std::ios::beg);
    if (!file) {
        throw std::runtime_error("Failed to skip past header");
    }

    std::array<char, 4> buffer;

    file.read(buffer.data(), buffer.size());
    if (!file) {
        throw std::runtime_error("Failed to read triangle count");
    }

    unsigned int triangle_count;
    std::memcpy(&triangle_count, buffer.data(), sizeof(triangle_count));

    vertices.reserve(triangle_count * 3);
    indices.reserve(triangle_count * 3);

    int index = 0;

    while (triangle_count > 0) {
        // Skip normal vector (12 bytes)
        file.seekg(12, std::ios::cur);

        for (int v = 0; v < 3; ++v) {
            float x, y, z;

            // Read x, y, z coordinates
            file.read(buffer.data(), buffer.size());
            if (!file) break;
            std::memcpy(&x, buffer.data(), sizeof(float));

            file.read(buffer.data(), buffer.size());
            if (!file) break;
            std::memcpy(&y, buffer.data(), sizeof(float));

            file.read(buffer.data(), buffer.size());
            if (!file) break;
            std::memcpy(&z, buffer.data(), sizeof(float));

            // Push vertex with hardcoded color
            vertices.push_back({glm::vec3(x, y, z), glm::vec3(0.3, 0.5, 0.4)});

            indices.push_back(index++);
        }

        // Skip attribute byte count (2 bytes)
        file.seekg(2, std::ios::cur);

        --triangle_count;

        if (!file) {
            std::cerr << "Error reading file data\n";
            break;
        }
    }

    file.close();
}


Mesh::~Mesh() {

}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
