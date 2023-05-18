#version 320 es

layout (location = 0) in vec2 p_pos;
layout (location = 1) in vec3 p_color;
layout (location = 2) in vec2 p_tpos;

out vec3 o_color;
out vec2 o_tpos;

void main()
{
    o_tpos = p_tpos;
    o_color = p_color;
    gl_Position = vec4(p_pos, 0.0, 1.0);
}