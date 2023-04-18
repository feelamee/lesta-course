#include <SDL3/SDL.h>

#include <event.hpp>
#include <iengine.hpp>

namespace nano
{

class engine final : public iengine
{
public:
    int  initialize(fs::path, fs::path) override;
    void finalize() override;
    void run() override;
    int  hot_reload() override;
    engine()
        : iengine()
    {
    }

private:
    SDL_Window* window         = nullptr;
    void* (*initialize_game)() = nullptr;
    void (*finalize_game)()    = nullptr;
    void*       libgame_handle = nullptr;
    fs::path    libgame;
    fs::path    tmp_libgame;
    std::string initialize_game_fname = "_Z15initialize_gamev";
    std::string finalize_game_fname   = "_Z13finalize_gamev";
};

} // namespace nano