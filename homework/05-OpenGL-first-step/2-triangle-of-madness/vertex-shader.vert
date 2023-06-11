#version 320 es

layout (location = 0) in vec2 position;

out vec4 color;

void main()
{
    gl_Position = vec4(position, 1, 1);
}
