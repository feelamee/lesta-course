#include <SDL2/SDL.h>
#include <iostream>

std::ostream& operator<<(std::ostream& out, const SDL_version& v)
{
    out << static_cast<int>(v.major) << '.';
    out << static_cast<int>(v.minor) << '.';
    out << static_cast<int>(v.patch);
    return out;
}

int main()
{
    SDL_Init(0);
    SDL_version compiled = {};
    SDL_version linked   = {};

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    std::cout << compiled << std::endl;
    std::cout << linked << std::endl;

    SDL_errorcode exit_code{};
    SDL_Error(exit_code);
    return exit_code | !std::cout.good();
}
