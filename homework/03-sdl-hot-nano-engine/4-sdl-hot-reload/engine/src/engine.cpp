#include <chrono>
using namespace std::literals::chrono_literals;
#include <cstdlib>
#include <iostream>
#include <thread>

#include <SDL3/SDL.h>

#include "engine.hpp"
#include <event.hpp>

namespace nano
{

int engine::initialize(fs::path libgame, fs::path tmp_libgame)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != EXIT_SUCCESS)
    {
        std::cerr << "engine::initialize SDL_Init error:\n\t" << SDL_GetError()
                  << std::endl;
        return EXIT_FAILURE;
    }
    window = SDL_CreateWindow("Test windows", 640, 480, SDL_WINDOW_VULKAN);
    if (window == NULL)
    {
        std::cerr << "engine::initialize SDL_CreateWindow error:\n\t"
                  << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    this->libgame     = libgame;
    this->tmp_libgame = tmp_libgame;
    if (hot_reload() == EXIT_FAILURE)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int engine::hot_reload()
{
    try
    {
        if (fs::exists(tmp_libgame) and
            fs::last_write_time(libgame) < fs::last_write_time(tmp_libgame))
            return EXIT_SUCCESS;

        fs::remove(tmp_libgame);
        fs::copy_file(libgame, tmp_libgame);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    std::this_thread::sleep_for(100ms);

    if (libgame_handle)
    {
        finalize_game();

        SDL_UnloadObject(libgame_handle);
        libgame_handle  = nullptr;
        finalize_game   = nullptr;
        initialize_game = nullptr;
    }

    void* new_libgame_handle = SDL_LoadObject(tmp_libgame.native().c_str());
    if (new_libgame_handle == NULL)
    {
        std::cerr << "[SDL_LoadObject] Failed to load shared object:\n"
                  << tmp_libgame.native() << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    libgame_handle  = new_libgame_handle;
    initialize_game = reinterpret_cast<decltype(initialize_game)>(
        SDL_LoadFunction(libgame_handle, initialize_game_fname.c_str()));
    finalize_game = reinterpret_cast<decltype(finalize_game)>(
        SDL_LoadFunction(libgame_handle, finalize_game_fname.c_str()));

    if (initialize_game == NULL or finalize_game == NULL)
    {
        std::cerr << "[SDL_LoadFunction] Failed to load function "
                  << initialize_game_fname << " or " << finalize_game_fname
                  << " from shared object " << tmp_libgame.native()
                  << std::endl;
        SDL_UnloadObject(libgame_handle);
        libgame_handle  = nullptr;
        finalize_game   = nullptr;
        initialize_game = nullptr;
        return EXIT_FAILURE;
    }

    initialize_game();
    return EXIT_SUCCESS;
}

void engine::finalize()
{
    finalize_game();
    SDL_UnloadObject(libgame_handle);
    libgame_handle  = nullptr;
    finalize_game   = nullptr;
    initialize_game = nullptr;

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void engine::run()
{
    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    bool is_running = true;
    while (is_running)
    {
        event ev;
        while (poll_event(&ev))
        {
            std::cout << "[" << duration_cast<milliseconds>(ev.key.timestamp)
                      << "]";
            switch (ev.type)
            {
            case EVENT_KEY_DOWN:
                std::cout << " EVENT_KEY_DOWN: "
                          << SDL_GetKeyName(ev.key.keysym.sym) << std::endl;
                break;

            case EVENT_KEY_UP:
                std::cout << " EVENT_KEY_UP: "
                          << SDL_GetKeyName(ev.key.keysym.sym) << std::endl;

                if (ev.key.keysym.sym == SDLK_q)
                    is_running = false;

                if (ev.key.keysym.sym == SDLK_r)
                    hot_reload();

                break;

            case EVENT_QUIT:
                std::cout << " EVENT_QUIT: " << std::endl;
                is_running = false;
                break;

            case EVENT_WINDOW_CLOSE_REQUESTED:
                std::cout << " EVENT_WINDOW_CLOSE_REQUESTED: " << std::endl;
                is_running = false;
                break;
            }
        }
    }
}

iengine& engine_instance()
{
    static engine nano{};
    iengine&      inano = nano;
    return inano;
}

} // namespace nano

int main(int argc, char const* argv[])
{
    nano::iengine& nano = nano::engine_instance();

    // check if name of libgame so was provided
    if (argc < 3 or !fs::exists(argv[1]))
    {
        std::cerr << "Too few arguments was provided\nOr libgame file not exist"
                  << std::endl;
        return EXIT_FAILURE;
    }

    int err_code =
        nano.initialize(fs::absolute(argv[1]), fs::absolute(argv[2]));
    if (err_code == EXIT_FAILURE)
    {
        std::cerr << "Engine init failed" << std::endl;
        return EXIT_FAILURE;
    }

    nano.run();
    nano.finalize();
    return 0;
}
