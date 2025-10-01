#version 460 core

in vec3 _aColor;
in vec2 _aTexCoord;
out vec4 FragColor;
uniform sampler2D ourTexture;

void main() {
    FragColor = vec4(_aColor, 1.0f);
    FragColor = texture(ourTexture, _aTexCoord);
}