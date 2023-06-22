#ifndef SCENE_HPP
#define SCENE_HPP

#include <nano/drawable.hpp>

#include <list>
#include <memory>

namespace nano
{

struct node : public drawable
{
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void pause();
    virtual void resume();
    virtual void process();

    virtual ~node() = default;
};

class scene : public node
{
public:
    void add_node(std::shared_ptr<node>);
    void erase_node(std::shared_ptr<node>);

    virtual void start() override;
    virtual void stop() override;
    virtual void pause() override;
    virtual void resume() override;

    virtual void process() override;

    virtual void draw(const drawable::state& attrs) const override;

private:
    std::list<std::shared_ptr<node>> nodes;
};

class scene_controller
{
public:
    using scene_ptr = std::shared_ptr<scene>;
    void push(scene_ptr);
    void pop();

private:
    std::list<scene_ptr> stack;
};

} // namespace nano

#endif // SCENE_HPP
