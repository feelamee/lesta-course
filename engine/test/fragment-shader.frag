#version 320 es
precision mediump float;

in vec2 tpos;

out vec4 color;

uniform sampler2D u_texture;

void main()
{
    color = texture(u_texture, tpos);
}



