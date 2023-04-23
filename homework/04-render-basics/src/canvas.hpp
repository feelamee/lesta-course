#include <cstddef>

class canvas
{
public:
    canvas(size_t p_width, size_t p_height)
        : width(p_width)
        , height(p_height)
    {
    }
private:
    size_t width = 0;
    size_t height = 0;
};
