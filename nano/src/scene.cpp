#include <nano/drawable.hpp>
#include <nano/scene.hpp>

#include <functional>

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
    using std::placeholders::_1;
    std::for_each(nodes.begin(), nodes.end(), std::bind(&node::start, _1));
}

void
scene::stop()
{
    using std::placeholders::_1;
    std::for_each(nodes.begin(), nodes.end(), std::bind(&node::stop, _1));
}

void
scene::pause()
{
    using std::placeholders::_1;
    std::for_each(nodes.begin(), nodes.end(), std::bind(&node::pause, _1));
}

void
scene::resume()
{
    using std::placeholders::_1;
    std::for_each(nodes.begin(), nodes.end(), std::bind(&node::resume, _1));
}

void
scene::process()
{
    using std::placeholders::_1;
    std::for_each(nodes.begin(), nodes.end(), std::bind(&node::process, _1));
}

void
scene::draw(const drawable::state& attrs) const
{
    using std::placeholders::_1;
    std::for_each(
        nodes.begin(), nodes.end(), std::bind(&node::draw, _1, attrs));
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
