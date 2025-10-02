#include "../include/fps_counter.h"

FPSCounter::FPSCounter()
    : prev_time(0.0), nb_frames(0) { }

void FPSCounter::update(GLFWwindow* window, std::string& scr_title) {
    double curr_time = glfwGetTime();
    ++nb_frames;

    if (curr_time - prev_time >= 1.0) {
        std::string scr_title_fps = scr_title + " - FPS: " + std::to_string(nb_frames);
        glfwSetWindowTitle(window, scr_title_fps.c_str());
        nb_frames = 0;
        prev_time = curr_time;
    }
}
