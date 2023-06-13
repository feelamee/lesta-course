#version 320 es

layout (location = 0) in vec2 p_pos;
layout (location = 1) in vec3 p_color;
layout (location = 2) in vec2 p_tpos;

uniform mat3 u_matrix;

out vec3 color;
out vec2 tpos;

void main()
{
    gl_Position = vec4(vec3(p_pos, 1.) * u_matrix, 1);
    color = p_color;
    tpos = p_tpos;
}



