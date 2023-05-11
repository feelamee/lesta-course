#ifndef IENGINE_HPP
#define IENGINE_HPP

#include <filesystem>

namespace nano
{

namespace fs = std::filesystem;

class iengine
{
public:
    iengine(const iengine&) = delete;
    iengine(iengine&&) = delete;
    iengine& operator=(const iengine&) = delete;
    iengine& operator=(iengine&&) = delete;

    virtual ~iengine()
    {
    }

    iengine()
    {
    }

    virtual int initialize(fs::path, fs::path) = 0;

    virtual void finalize() = 0;

    virtual void run() = 0;
};

iengine& engine_instance();

} // namespace nano

#endif // IENGINE_HPP
