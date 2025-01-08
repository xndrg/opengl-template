#version 330 core

in vec2 tex_coords;
out vec4 final_color;

uniform sampler2D my_texture;

void main()
{
    final_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
