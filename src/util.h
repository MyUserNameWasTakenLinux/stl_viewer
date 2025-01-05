#ifndef UTIL_H
#define UTIL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <filesystem>

struct Shader {
    unsigned int id; // shader id

    Shader(std::filesystem::path vs_path, std::filesystem::path fs_path);

    void use();

    void set_bool(std::string_view name, bool value) const;

    void set_int(std::string_view name, int value) const;

    void set_float(std::string_view name, float value) const;

    void set_vec2(std::string_view name, glm::vec2 value) const;

    void set_vec2(std::string_view name, float x, float y) const;

    void set_vec3(std::string_view name, glm::vec3 value) const;

    void set_vec3(std::string_view name, float x, float y, float z) const;

    void set_vec4(std::string_view name, glm::vec4 value) const;

    void set_vec4(std::string_view name, float x, float y, float z, float w) const;

    void set_mat2(std::string_view name, glm::mat2 value) const;

    void set_mat3(std::string_view name, glm::mat3 value) const;

    void set_mat4(std::string_view name, glm::mat4 value) const;

    void check_compile_error(GLuint id, std::string type) const;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int VAO;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        Mesh(std::filesystem::path stl_path);

        ~Mesh();

        void draw() const;

    private:
        unsigned int VBO, EBO;
};

#endif