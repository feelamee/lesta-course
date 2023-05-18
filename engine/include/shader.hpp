#ifndef SHADER_HPP
#define SHADER_HPP

namespace nano
{
using uint = unsigned int;

enum shader_t
{
    fragment = 0x8B30,
    vertex = 0x8B31,
};

uint create_shader(shader_t type,
                   const char* src,
                   char* compile_info,
                   uint buf_size);

uint setup_shaders(const char* vertex_shader_src_path,
                   const char* fragment_shader_src_path);

} // namespace nano

#endif // SHADER_HPP
