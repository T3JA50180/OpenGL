#version 460 core

in vec3 _aColor;
in vec2 _aTexCoord;
out vec4 FragColor;
uniform sampler2D ourTexture;
uniform sampler2D ourTexture0;

void main() {
    FragColor = vec4(_aColor, 1.0f);
    FragColor = mix(texture(ourTexture, _aTexCoord), texture(ourTexture0, _aTexCoord), 0.5);
}