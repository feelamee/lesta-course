#include <chrono>
#include <cstdlib>
#include <iostream>

#include <SDL3/SDL.h>

#include <engine.hpp>
#include <event.hpp>

namespace nano
{

class engine : public iengine
{
public:
  int initialize () override;
  void finalize () override;
  void run () override;
  engine () : iengine () {}

private:
  SDL_Window *window = nullptr;
};

int
engine::initialize ()
{
  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_EVENTS) != EXIT_SUCCESS)
    {
      std::cerr << "engine::initialize SDL_Init error:\n\t" << SDL_GetError ()
                << std::endl;
      return EXIT_FAILURE;
    }
  window = SDL_CreateWindow ("Test windows", 640, 480, SDL_WINDOW_VULKAN);
  if (window == NULL)
    {
      std::cerr << "engine::initialize SDL_CreateWindow error:\n\t"
                << SDL_GetError () << std::endl;
      SDL_Quit ();
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

void
engine::finalize ()
{
  SDL_DestroyWindow (window);
  SDL_Quit ();
}

void
engine::run ()
{
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;

  bool is_running = true;
  while (is_running)
    {
      event ev;
      while (poll_event (&ev))
        {
          switch (ev.type)
            {
            case EVENT_KEY_DOWN:
            case EVENT_KEY_UP:
              std::cout << "["
                        << duration_cast<milliseconds> (
                               duration<std::uint64_t, std::nano> (
                                   ev.key.timestamp))
                        << (ev.type == EVENT_KEY_DOWN ? "] EVENT_KEY_DOWN: "
                                                      : "] EVENT_KEY_UP: ")
                        << SDL_GetKeyName (ev.key.keysym.sym) << std::endl;

              if (ev.key.keysym.sym == SDLK_q)
                is_running = false;
              break;

            case EVENT_QUIT:
            case EVENT_WINDOW_CLOSE_REQUESTED:
              std::cout << "[" << ev.key.timestamp << "]"
                        << (ev.type == EVENT_QUIT
                                ? " EVENT_QUIT: "
                                : " EVENT_WINDOW_CLOSE_REQUESTED: ")
                        << std::endl;
              is_running = false;
              break;
            }
        }
    }
}

iengine &
engine_instance ()
{
  static engine nano{};
  iengine &inano = nano;
  return inano;
}

} // namespace nano