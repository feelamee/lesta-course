#ifndef IENGINE_HPP
#define IENGINE_HPP

#include <nano/transform.hpp>
#include <nano/vertbuf.hpp>

#include <filesystem>
#include <string>

namespace nano
{

class iengine
{
public:
    virtual ~iengine() = default;

    virtual int initialize() = 0;
    virtual void finalize() = 0;
    virtual int swap_buffers() = 0;
};

iengine& engine_instance();

} // namespace nano

#endif // IENGINE_HPP
