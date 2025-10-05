#include <iostream>

#include "../include/gl.h"

#include "../include/fps_counter.h"
#include "../include/glfw_window.h"
#include "../include/shader.h"
#include "../include/stb_image.h"
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
         0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,    2.0f, 2.0f,
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    2.0f, -1.0f,
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    -1.0f, -1.0f,
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,    -1.0f, 2.0f,
    };

    constexpr GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Load shader.
    Shader shader1("../shaders/shader.vert", "../shaders/shader.frag");

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Start recording state in VAO.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Do not unbind EBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Stop setting state in VAO.
    glBindVertexArray(0);

    // Load texture from file.
    Texture texture0("../assets/textures/imagetex.jpg");
    Texture texture1("../assets/textures/brick_wall.jpg");

    shader1.useProgram();
    // Do this after the above line only.
    glBindTextureUnit(0, texture0.texture);
    shader1.setInt("ourTexture0", 0);
    glBindTextureUnit(1, texture1.texture);
    shader1.setInt("ourTexture", 1);

    FPSCounter fps_counter;
    // glfwSwapInterval(0);

    while (!glfwWindowShouldClose(window)) {
        fps_counter.update(window, kScrTitle);
        processInput(window, shader1);

        glClearColor(66 / 255.0f, 135 / 255.0f, 245 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader1.useProgram();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up.
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glDeleteTextures(1, &texture0.texture);
    shader1.deleteProgram();

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
