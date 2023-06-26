#ifndef TETRAMINO_HPP
#define TETRAMINO_HPP

#include <nano/scene.hpp>
#include <nano/sprite.hpp>

#include <vector>

namespace tetris
{

class tetramino : public nano::node
{
public:
    enum class type
    {
        I,
        O,
        T,
        L,
        J,
        S,
        Z
    };
    static type random_type();

    enum class state
    {
        falling,
        locked,
    };

    tetramino(type, nano::vec2f);

    void start() override;
    void stop() override;

    void pause() override;
    void resume() override;
    void process(delta_t) override;

    void lock();
    bool is_locked() const;
    bool is_falling() const;

    void rot90();
    void rot270();
    nano::vec2f origin();

    void xshift(const int step);
    void yshift(const int step);

    void remove(const nano::vec2i);

    void draw(nano::drawable::state) const override;

    static constexpr std::size_t max_positions_num = 4;
    std::vector<nano::vec2i> positions() const;
    std::vector<nano::vec2i>& positions();

    friend bool is_collide(const std::shared_ptr<tetramino> lhs,
                           const std::shared_ptr<tetramino> rhs);

    friend bool is_collide(const std::shared_ptr<tetramino> lhs);

private:
    mutable nano::sprite block;
    type t;
    nano::vec2f m_origin{};
    state m_state;
    std::vector<nano::vec2i> blocks_positions{};
};

} // namespace tetris

#endif // TETRAMINO_HPP
