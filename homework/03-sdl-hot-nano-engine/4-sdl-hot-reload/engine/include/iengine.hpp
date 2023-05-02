#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <filesystem>
namespace fs = std::filesystem;

namespace nano
{

class iengine
{
public:
    iengine(const iengine&)            = delete;
    iengine(iengine&&)                 = delete;
    iengine& operator=(const iengine&) = delete;
    iengine& operator=(iengine&&)      = delete;

    virtual ~iengine() {}
    iengine() {}
    virtual int  initialize(fs::path, fs::path) = 0;
    virtual void finalize()                     = 0;
    virtual void run()                          = 0;
    virtual int  hot_reload()                   = 0;
};

iengine& engine_instance();

} // namespace nano

#endif // ENGINE_HPP