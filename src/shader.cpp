#include "../include/shader.h"
#include <fstream>
#include <iostream>

Shader::Shader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path) {
    // Retrieve the vertex and fragment shader source code.
    std::string vertex_shader_code_str;
    std::string fragment_shader_code_str;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        std::stringstream vertex_shader_stream;
        std::stringstream fragment_shader_stream;

        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_shader_code_str = vertex_shader_stream.str();
        fragment_shader_code_str = fragment_shader_stream.str();
    } catch (const std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }

    // Compile shaders.
    const GLchar* vertex_shader_code = vertex_shader_code_str.c_str();
    const GLchar* fragment_shader_code = fragment_shader_code_str.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex_shader);
    checkCompileErrors(vertex_shader, "VERTEX");

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);
    glCompileShader(fragment_shader);
    checkCompileErrors(fragment_shader, "FRAGMENT");

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    checkCompileErrors(shader_program, "PROGRAM");

    // Clean up.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::useProgram() {
    glUseProgram(shader_program);
}

void Shader::setBool(std::string &name, bool value) {
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniform1i(location, (GLint)value);
}

void Shader::setInt(std::string &name, int value) {
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniform1i(location, (GLint)value);
}

void Shader::setFloat(std::string &name, float value) {
    GLint location = glGetUniformLocation(shader_program, name.c_str());
    glUniform1f(location, (GLfloat)value);
}

void Shader::deleteProgram() {
    glDeleteProgram(shader_program);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[512];

    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetProgramInfoLog(shader, 512, &success, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetShaderInfoLog(shader, 512, &success, infoLog);
            std::cerr << "ERROR::SHADER::" << type << "::COMPILE::FAILED\n" << infoLog << std::endl;
        }
    }
}