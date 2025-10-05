#include "../include/texture.h"

#include <fstream>
#include <iostream>

#include "../include/stb_image.h"

Texture::Texture(std::filesystem::path texture_data_path)
    : texture_data_path(std::move(texture_data_path)) {
    if (loadTexture()) {
        setTextureParameters();
    } else {
        texture = 0;
    }
}

Texture::~Texture() {
    deleteTexture();
}

void Texture::deleteTexture() {
    if (texture != 0) {
        glDeleteTextures(1, &texture);
    }
}

bool Texture::loadTexture() {
    int width, height, nr_channels;
    stbi_uc* texture_data = stbi_load(texture_data_path.c_str(), &width, &height, &nr_channels, 0);
    if (!texture_data) {
        std::cout << "Failed to load texture from " << texture_data_path << std::endl;
        return false;
    }

    GLenum internal_format = nr_channels == 4 ? GL_RGBA8 : GL_RGB8;
    GLenum format = nr_channels == 4 ? GL_RGBA : GL_RGB;

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureStorage2D(texture, 1, internal_format, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texture_data);

    stbi_image_free(texture_data);
    return true;
}

void Texture::setTextureParameters() {
    glGenerateTextureMipmap(texture);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
