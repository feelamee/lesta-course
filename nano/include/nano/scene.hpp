#ifndef SCENE_HPP
#define SCENE_HPP

#include <nano/drawable.hpp>

#include <chrono>
#include <list>
#include <memory>

namespace nano
{

class node : public drawable
{
public:
    node();
    int id{ 0 };
    static int last_id;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    using delta_t = std::chrono::nanoseconds;
    virtual void process(delta_t) = 0;

    virtual ~node() = default;
};

class scene_controller
{
public:
    using scene_ptr = std::shared_ptr<node>;
    void push(scene_ptr);
    void pop();
    scene_ptr top();

private:
    std::list<scene_ptr> stack;
};

} // namespace nano

#endif // SCENE_HPP
