#ifndef OPENGL_FPS_COUNTER_H
#define OPENGL_FPS_COUNTER_H

#include <string>

#include <GLFW/glfw3.h>

class FPSCounter {
public:
    FPSCounter();
    void update(GLFWwindow* window, std::string& scr_title);

private:
    double prev_time;
    int nb_frames;
};

#endif