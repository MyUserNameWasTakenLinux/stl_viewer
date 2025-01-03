#ifndef UTIL_H
#define UTIL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <filesystem>

struct Shader {
    unsigned int id; // shader id

    Shader(std::filesystem::path vs_path, std::filesystem::path fs_path);

    void use();

    // void set_bool();

    // void set_int();

    // void set_float();

    void check_compile_error(GLuint id, std::string type);
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

        ~Mesh();

        void draw(Shader& s);

    private:
        unsigned int VBO, EBO;
};

#endif