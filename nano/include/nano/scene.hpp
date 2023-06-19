#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <nano/drawable.hpp>

#include <list>

namespace nano
{

struct node : drawable
{
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void pause();
    virtual void resume();
    virtual void process();

    virtual ~node() = default;
};

struct scene : node
{
    std::list<std::shared_ptr<node>> nodes;

    void add_node(std::shared_ptr<node>);
    void erase_node(std::shared_ptr<node>);

    virtual void start() override;
    virtual void stop() override;
    virtual void pause() override;
    virtual void resume() override;

    virtual void process() override;

    virtual void draw(const drawable::state& attrs) const override;
};

} // namespace nano

#endif // SCENE_HPP
