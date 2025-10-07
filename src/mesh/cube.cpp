#include "../../include/mesh/cube.h"

#include <array>

Cube::Cube(float half_size)
    : half_size(half_size) {
    generateVertices();
    generateIndices();
}

std::vector<Vertex>& Cube::getVertices() {
    return vertices;
}

std::vector<GLuint>& Cube::getIndices() {
    return indices;
}

void Cube::generateVertices() {
    std::array<glm::vec3, 6> normals = {{
        { 0.0f,  0.0f,  1.0f}, // Front.
        { 0.0f,  0.0f, -1.0f}, // Back.
        {-1.0f,  0.0f,  0.0f}, // Left.
        { 1.0f,  0.0f,  0.0f}, // Right.
        { 0.0f,  1.0f,  0.0f}, // Top.
        { 0.0f, -1.0f,  0.0f}, // Bottom.
    }};

    for (glm::vec3& n : normals) {
        glm::vec3 c, u, v;

        // Calculate local axes for the face.
        if (std::abs(n.x) > 0.5f) {
            u = {0.0f, 1.0f, 0.0f};
            v = {0.0f, 0.0f, 1.0f};
        } else if (std::abs(n.y) > 0.5f) {
            u = {1.0f, 0.0f, 0.0f};
            v = {0.0f, 0.0f, 1.0f};
        } else {
            u = {1.0f, 0.0f, 0.0f};
            v = {0.0f, 1.0f, 0.0f};
        }

        c = n * half_size;
        u *= half_size;
        v *= half_size;

        vertices.push_back({c - u - v, n, {0.0f, 0.0f}}); // Bottom-Left.
        vertices.push_back({c - u + v, n, {0.0f, 1.0f}}); // Top-Left.
        vertices.push_back({c + u + v, n, {1.0f, 1.0f}}); // Top-Right.
        vertices.push_back({c + u - v, n, {1.0f, 0.0f}}); // Bottom-Right.
    }
}

void Cube::generateIndices() {
    for (GLuint i = 0; i < 6; i++) {
        GLuint offset = i * 4;
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);
        indices.push_back(offset + 0);
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
    }
}
