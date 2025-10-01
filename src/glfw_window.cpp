#include <iostream>

#include "../include/gl.h"
#include "../include/glfw_window.h"

GLFWwindow* initGlfwWindow(int width, int height, std::string& title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to initialize GLFW window." << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Load all OpenGL function pointers.
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr<< "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return nullptr;
    }
    return window;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
