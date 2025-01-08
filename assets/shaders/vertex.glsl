#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex_coords;

out vec2 tex_coords;

uniform mat4 trans;

void main()
{
    tex_coords = a_tex_coords;
    gl_Position = trans * vec4(a_pos, 1.0f);
}
