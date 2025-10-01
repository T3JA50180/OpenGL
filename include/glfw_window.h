#ifndef OPENGL_GLFW_WINDOW_H
#define OPENGL_GLFW_WINDOW_H

#include <string>

#include <GLFW/glfw3.h>

GLFWwindow* initGlfwWindow(int width, int height, std::string& title);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif