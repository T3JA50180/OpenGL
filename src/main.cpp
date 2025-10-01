#include <iostream>

#include "../include/gl.h"

#include "../include/glfw_window.h"
#include "../include/shader.h"
#include "../include/stb_image.h"
#include "../include/fps_counter.h"

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

    // Load shader.
    Shader shader1("../shaders/shader.vert", "../shaders/shader.frag");

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Load texture.
    GLuint texture1, texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nr_channels;
    stbi_uc* data = stbi_load("../assets/textures/brick_wall.jpg", &width, &height, &nr_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture.\n" << std::endl;
    }
    stbi_image_free(data);

    shader1.useProgram();
    std::string tex = "texture1";
    shader1.setInt(tex, 1);

    // FPS counter.
    FPSCounter fps_counter;
    glfwSwapInterval(0);

    // Render loop.
    while (!glfwWindowShouldClose(window)) {
        // FPS counter.
        fps_counter.update(window, kScrTitle);
        processInput(window, shader1);

        glClearColor(66 / 255.0f, 135 / 255.0f, 245 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

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
