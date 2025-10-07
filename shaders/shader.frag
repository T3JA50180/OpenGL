#version 460 core

in vec2 texture_pos_out;

out vec4 frag_color;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

void main() {
     frag_color = mix(texture(texture_0, texture_pos_out), texture(texture_1, texture_pos_out), 0.75);
}