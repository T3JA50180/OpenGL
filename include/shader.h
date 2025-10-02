#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <filesystem>
#include <string>

#include "gl.h"

class Shader {
public:
    GLuint shader_program;
    std::filesystem::path vertex_shader_path;
    std::filesystem::path fragment_shader_path;

    Shader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);
    ~Shader();

    void useProgram();
    void deleteProgram();
    void reloadProgram();

    void setBool(std::string_view name, bool value);
    void setInt(std::string_view name, int value);
    void setFloat(std::string_view name, float value);

private:
    bool compileAndLink(GLuint& shader_program);
    bool compileSucceeded(GLuint shader, std::string type);
};

#endif