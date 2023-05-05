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

    void clear(color = colors::black) override;

    canvas&
    data() override;

    std::vector<position> line(position, position) override;

    std::vector<vertex>
    line(const vertex& v1, const vertex& v2);

    void draw(position, position, color) override;

    void
    draw(const std::vector<vertex>&);

    static position
    interpolate(const position&, const position&, float);

    static vertex
    interpolate(const vertex&, const vertex&, float);
};

#endif // LINE_RENDER_HPP
