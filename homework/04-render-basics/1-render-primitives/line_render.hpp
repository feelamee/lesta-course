#ifndef LINE_RENDER_HPP
#define LINE_RENDER_HPP

#include "canvas.hpp"
#include "render.hpp"
#include "shader.hpp"
#include "vertex.hpp"

#include <boost/numeric/ublas/matrix.hpp>
#include <ranges>
#include <vector>
using namespace boost::numeric;

class line_render : public render
{
private:
    canvas& buf;

public:
    std::shared_ptr<shader> program = std::make_shared<dummy>();

    line_render(canvas& _buf)
        : buf(_buf)
    {
    }

    void
    clear(color color_ = colors::black) override;

    canvas&
    data() override;

    std::vector<position>
    line(position p1, position p2) override;

    void
    draw(position p1, position p2, color p_color) override;

    void
    draw(const std::vector<vertex>& vertices);
};

#endif // LINE_RENDER_HPP
