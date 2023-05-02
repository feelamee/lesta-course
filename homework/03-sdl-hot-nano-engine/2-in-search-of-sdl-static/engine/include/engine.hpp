#ifndef ENGINE_HPP
#define ENGINE_HPP

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
    virtual int  initialize() = 0;
    virtual void finalize()   = 0;
    virtual void run()        = 0;
};

iengine& engine_instance();

} // namespace nano

#endif // ENGINE_HPP