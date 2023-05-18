#ifndef IENGINE_HPP
#define IENGINE_HPP

#include <filesystem>
#include <string>
#include <vertbuf.hpp>

namespace nano
{

class engine;
struct texture;

class iengine
{
public:
    virtual ~iengine()
    {
    }

    iengine()
    {
    }

    virtual int initialize() = 0;
    virtual void finalize() = 0;
    virtual void set_program(uint) = 0;
    virtual void set_uniform(const std::string&, const texture*) = 0;
    virtual int swap_buffers() = 0;
};

iengine& engine_instance();

} // namespace nano

#endif // IENGINE_HPP
