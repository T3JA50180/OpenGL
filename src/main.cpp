#include <iostream>
#include <vector>

#include "../include/gl.h"

#include "../include/fps_counter.h"
#include "../include/glfw_window.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/mesh/cube.h"
#include "../include/shader.h"
#include "../include/texture.h"

void processInput(GLFWwindow* window, Shader& shader);

constexpr int kScrWidth = 800;
constexpr int kScrHeight = 600;
std::string kScrTitle = "OpenGL";

int main() {
    GLFWwindow* window = initGlfwWindow(kScrWidth, kScrHeight, kScrTitle);
    if (window == nullptr) {
        return -1;
    }

    std::vector<GLfloat> vertex_buffer_data;
    std::vector<GLuint> indices_data;

    // Procedural cube creation.
    Cube cube_1 = Cube();
    std::vector<Vertex> cube1_vertices = cube_1.getVertices();
    std::vector<GLuint> cube1_indices = cube_1.getIndices();

    for (Vertex& v : cube1_vertices) {
        vertex_buffer_data.push_back(v.position.x);
        vertex_buffer_data.push_back(v.position.y);
        vertex_buffer_data.push_back(v.position.z);
        vertex_buffer_data.push_back(v.uv.x);
        vertex_buffer_data.push_back(v.uv.y);
    }

    for (GLuint i : cube1_indices) {
        indices_data.push_back(i);
    }

    // vertex_buffer_data.resize(20);
    // indices_data.resize(6);

    GLuint vao, vbo, ebo;

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);

    // Upload vertex data.
    glNamedBufferData(vbo, vertex_buffer_data.size() * sizeof(GLfloat), vertex_buffer_data.data(), GL_STATIC_DRAW);
    // Upload index data.
    glNamedBufferData(ebo, indices_data.size() * sizeof(GLint), indices_data.data(), GL_STATIC_DRAW);

    // Associate the EBO with the VAO.
    glVertexArrayElementBuffer(vao, ebo);

    // Define vertex attributes directly on the VAO.
    // Position.
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    // Texture coordinates.
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
    glVertexArrayAttribBinding(vao, 1, 0);

    // Bind the VBO to binding index 0 of the VAO.
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 5 * sizeof(GLfloat));

    // Load shader.
    Shader shader_1 = Shader("../shaders/shader.vert", "../shaders/shader.frag");

    // Load textures from file.
    Texture texture_0 = Texture("../assets/textures/brick_wall.jpg");
    Texture texture_1 = Texture("../assets/textures/smiley_face.jpg");

    shader_1.useProgram();
    // Do this after the above line only.
    glBindTextureUnit(0, texture_0.texture);
    shader_1.setInt("texture_0", 0);
    glBindTextureUnit(1, texture_1.texture);
    shader_1.setInt("texture_1", 1);

    FPSCounter fps_counter;
    glfwSwapInterval(0);

    std::vector<glm::vec3> cube_positions = {
        { 0.0f,  0.0f,  0.0f},
        { 2.0f,  5.0f, -15.0f},
        {-1.5f, -2.2f, -2.5f},
        {-3.8f, -2.0f, -12.3f},
        { 2.4f, -0.4f, -3.5f},
        {-1.7f,  3.0f, -7.5f},
        { 1.3f, -2.0f, -2.5f},
        { 1.5f,  2.0f, -2.5f},
        { 1.5f,  0.2f, -1.5f},
        {-1.3f,  1.0f, -1.5f}
    };

    while (!glfwWindowShouldClose(window)) {
        fps_counter.update(window, kScrTitle);
        processInput(window, shader_1);

        glClearColor(66 / 255.0f, 135 / 255.0f, 245 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);

        // Get location of uniform variables.
        GLint model_location = glGetUniformLocation(shader_1.shader_program, "model");
        GLint view_location = glGetUniformLocation(shader_1.shader_program, "view");
        GLint projection_location = glGetUniformLocation(shader_1.shader_program, "projection");

        // Transform.
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

        for (int i = 0; i < cube_positions.size(); i++) {
            float time = glfwGetTime();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            model = glm::rotate(model, glm::radians(time * 90.0f), glm::vec3(sin(time * i), cos(time * i), tan(time * i)));
            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up.
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, Shader& shader) {
    static bool r_key_pressed_last_frame = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (r_key_pressed_last_frame == false) {
            shader.reloadProgram();
        }
        r_key_pressed_last_frame = true;
    } else {
        r_key_pressed_last_frame = false;
    }
}
