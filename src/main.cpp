#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/shader.h"

void framebufferSizeCallback(GLFWwindow*, GLsizei, GLsizei);
void processInput(GLFWwindow*);

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;
std::string SCR_TITLE = "OpenGL";

int main() {
    // glfw window creation.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to initialize GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Load all OpenGL function pointers.
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr<< "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return -1;
    }

    Shader shader1("../shaders/shader.vert", "../shaders/shader.frag");

    // Vertex data.
    constexpr GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    constexpr GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

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

    // FPS counter.
    double prevTime = glfwGetTime();
    int nbFrames = 0;

    // Render loop.
    while (!glfwWindowShouldClose(window)) {
        // FPS counter.
        double currTime = glfwGetTime();
        ++nbFrames;

        if (currTime - prevTime >= 1.0) {
            std::string SCR_TITLE_FPS = SCR_TITLE + " - FPS: " + std::to_string(nbFrames);
            glfwSetWindowTitle(window, SCR_TITLE_FPS.c_str());
            prevTime += 1.0;
            nbFrames = 0;
        }
        processInput(window);

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
    shader1.deleteProgram();

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


