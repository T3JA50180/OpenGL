#ifndef OPENGL_CUBE_H
#define OPENGL_CUBE_H

#include <vector>

#include "../gl.h"
#include "../glm/glm.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Cube {
public:
    explicit Cube(float half_size = 0.5f);
    std::vector<Vertex>& getVertices();
    std::vector<GLuint>& getIndices();

private:
    float half_size;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    void generateVertices();
    void generateIndices();
};

#endif
