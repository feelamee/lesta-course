#include "nano/drawable.hpp"
#include <nano/scene.hpp>

#include <functional>
#include <ranges>

namespace nano
{

void
node::pause()
{
}

void
node::resume()
{
}

void
node::process()
{
}

void
scene::start()
{
    std::ranges::for_each(nodes, &node::start);
}

void
scene::stop()
{
    std::ranges::for_each(nodes, &node::stop);
}

void
scene::pause()
{
    std::ranges::for_each(nodes, &node::pause);
}

void
scene::resume()
{
    std::ranges::for_each(nodes, &node::resume);
}

void
scene::process()
{
    std::ranges::for_each(nodes, &node::process);
}

void
scene::draw(const drawable::state& attrs) const
{
    using std::placeholders::_1;
    auto&& binder = std::bind(&node::draw, _1, attrs);
    std::ranges::for_each(nodes, binder);
}

void
scene::add_node(std::shared_ptr<node> p_node)
{
    nodes.push_back(p_node);
}

void
scene::erase_node(std::shared_ptr<node> p_node)
{
    nodes.remove(p_node);
}

} // namespace nano
