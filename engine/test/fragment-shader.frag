#version 320 es
precision mediump float;


out vec4 o_color;
// in vec2 p_tpos;

// uniform sampler2D p_texture;

void main()
{
    // o_color = texture(p_texture, p_tpos);
    o_color = vec4(0, 0, 1, 1);
}