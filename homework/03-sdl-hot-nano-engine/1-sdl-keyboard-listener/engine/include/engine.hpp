#ifndef ENGINE_HPP
#define ENGINE_HPP

struct SDL_Window;

namespace nano
{

class iengine
{
  iengine () {}

  SDL_Window *window;

public:
  static iengine &instance ();

  iengine (const iengine &) = delete;
  iengine (iengine &&) = delete;
  iengine &operator= (const iengine &) = delete;
  iengine &operator= (iengine &&) = delete;

  ~iengine () {}
  int initialize ();
  void finalize ();
  void run ();
};

} // namespace nano

#endif // ENGINE_HPP