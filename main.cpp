#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;
std::string SCR_TITLE = "OpenGL";

void setFramebufferSizeCallback(GLFWwindow*, GLsizei, GLsizei);
void processInput(GLFWwindow*);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to initialize GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, setFramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout<< "Failed to initialize GLAD." << std::endl;
        glfwTerminate();
        return -1;
    }

    double prevTime = glfwGetTime();
    int nbFrames = 0;

    while (!glfwWindowShouldClose(window)) {
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void setFramebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


