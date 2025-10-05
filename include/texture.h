#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <filesystem>

#include "gl.h"

class Texture {
public:
    GLuint texture;
    std::filesystem::path texture_data_path;

    explicit Texture(std::filesystem::path texture_data_path);
    ~Texture();

    void deleteTexture();
private:
    bool loadTexture();
    void setTextureParameters();
};

#endif