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

    enum class rotation
    {
        _90,
        _180,
        _270,
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

    void rshift();
    void lshift();

    nano::vec2i min_positionX() const;
    nano::vec2i min_positionY() const;

    nano::vec2i max_positionX() const;
    nano::vec2i max_positionY() const;

    void draw(nano::drawable::state) const override;

    static constexpr std::size_t max_positions_num = 4;
    std::vector<nano::vec2i> positions() const;

    friend bool is_collideY(const std::shared_ptr<tetramino> lhs,
                            const std::shared_ptr<tetramino> rhs);

    friend bool is_collideX(const std::shared_ptr<tetramino> lhs,
                            const std::shared_ptr<tetramino> rhs);

    bool is_on_floor() const;
    bool is_wall_left() const;
    bool is_wall_right() const;

private:
    mutable nano::sprite block;
    type t;
    int rotation_num{ 0 };
    state m_state;
    std::vector<nano::vec2i> blocks_positions{};
};

} // namespace tetris

#endif // TETRAMINO_HPP
