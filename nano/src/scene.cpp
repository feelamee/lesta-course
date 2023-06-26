#include <chrono>
#include <nano/drawable.hpp>
#include <nano/scene.hpp>

#include <functional>

namespace nano
{

int node::last_id{ 0 };

node::node()
    : id(last_id)
{
    last_id++;
}

//////////////////////////////////////////////////////

void
scene_controller::push(scene_ptr scene)
{
    if (not stack.empty())
    {
        stack.back()->pause();
    }

    stack.push_back(scene);
    stack.back()->start();
}

void
scene_controller::pop()
{
    if (not stack.empty())
    {
        stack.back()->stop();
    }

    stack.pop_back();

    if (not stack.empty())
    {
        stack.back()->resume();
    }
}

scene_controller::scene_ptr
scene_controller::top()
{
    return stack.back();
}

} // namespace nano
