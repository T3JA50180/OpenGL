#include <iostream>

#include "../include/gl.h"

#include "../include/fps_counter.h"
#include "../include/glfw_window.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"
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

    // Vertex data.
    constexpr GLfloat vertices[] = {
        // positions.          // colors.           // texture.
         0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
    };

    constexpr GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint vao, vbo, ebo;

    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);

    // Upload vertex data.
    glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Upload index data.
    glNamedBufferData(ebo, sizeof(indices), indices, GL_STATIC_DRAW);

    // Associate the EBO with the VAO.
    glVertexArrayElementBuffer(vao, ebo);

    // Define vertex attributes directly on the VAO.
    // Position.
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    // Color.
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
    glVertexArrayAttribBinding(vao, 1, 0);

    // Texture coordinates.
    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribFormat(vao, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat));
    glVertexArrayAttribBinding(vao, 2, 0);

    // Bind the VBO to binding index 0 of the VAO.
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 8 * sizeof(GLfloat));

    // Load shader.
    Shader shader1 = Shader("../shaders/shader.vert", "../shaders/shader.frag");

    // Load textures from file.
    Texture texture0 = Texture("../assets/textures/brick_wall.jpg");
    Texture texture1 = Texture("../assets/textures/smiley_face.jpg");

    shader1.useProgram();
    // Do this after the above line only.
    glBindTextureUnit(0, texture0.texture);
    shader1.setInt("ourTexture0", 0);
    glBindTextureUnit(1, texture1.texture);
    shader1.setInt("ourTexture", 1);

    FPSCounter fps_counter;
    glfwSwapInterval(0);

    while (!glfwWindowShouldClose(window)) {
        fps_counter.update(window, kScrTitle);
        processInput(window, shader1);

        glClearColor(66 / 255.0f, 135 / 255.0f, 245 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        GLint transform_location = glGetUniformLocation(shader1.shader_program, "transform");
        float time = glfwGetTime();
        // Transform.
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(-cos(time), -cos(time), -cos(time)));
        transform = glm::scale(transform, glm::vec3(sin(time), sin(time), sin(time)));
        transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(sin(time), sin(time), sin(time)));
        transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
        transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
