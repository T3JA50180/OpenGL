#include <fstream>
#include <iostream>

#include "../include/shader.h"

Shader::Shader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path)
    : vertex_shader_path(vertex_shader_path), fragment_shader_path(fragment_shader_path) {
    GLuint initial_shader_program;
    if (compileAndLink(initial_shader_program)) {
        shader_program = initial_shader_program;
    } else {
        shader_program = 0;
    }
}

Shader::~Shader() {
    deleteProgram();
}

void Shader::useProgram() {
    if (shader_program != 0) {
        glUseProgram(shader_program);
    }
}

void Shader::deleteProgram() {
    if (shader_program != 0) {
        glDeleteProgram(shader_program);
        shader_program = 0;
    }
}

void Shader::reloadProgram() {
    GLuint reloaded_shader_program;
    if (compileAndLink(reloaded_shader_program)) {
        shader_program = reloaded_shader_program;
        std::cout << "Shaders reloaded successfully.\n";
    } else {
        std::cerr << "Shader reload failed. Keeping previous shader program.\n";
    }
}

void Shader::setBool(std::string_view name, bool value) {
    GLint location = glGetUniformLocation(shader_program, name.data());
    glUniform1i(location, (GLint)value);
}

void Shader::setInt(std::string_view name, int value) {
    GLint location = glGetUniformLocation(shader_program, name.data());
    glUniform1i(location, (GLint)value);
}

void Shader::setFloat(std::string_view name, float value) {
    GLint location = glGetUniformLocation(shader_program, name.data());
    glUniform1f(location, (GLfloat)value);
}


bool Shader::compileAndLink(GLuint& shader_program) {
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
    if (!compileSucceeded(vertex_shader, "VERTEX")) {
        return false;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);
    glCompileShader(fragment_shader);
    if (!compileSucceeded(fragment_shader, "FRAGMENT")) {
        return false;
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    if (!compileSucceeded(shader_program, "PROGRAM")) {
        return false;
    }

    // Clean up.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return true;
}

bool Shader::compileSucceeded(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[512];

    if (type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            glGetProgramInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            return false;
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::" << type << "::COMPILE::FAILED\n" << infoLog << std::endl;
            return false;
        }
    }
    return true;
}