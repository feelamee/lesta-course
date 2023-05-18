#version 320 es
precision mediump float;

in vec2 p_tpos;

out vec4 o_color;

uniform sampler2D p_texture;

void main()
{
    o_color = texture(p_texture, p_tpos);
}