#ifndef TRANSOFRMABLE_HPP
#define TRANSOFRMABLE_HPP

#include <cstddef>
#include <vec.hpp>

namespace nano
{

class transform
{
public:
    /**
     * @brief m_01, where 0 is column and 1 is row (like x and y)
     */
    // clang-format off
    transform(float m_00, float m_01, float m_02,
              float m_10, float m_11, float m_12,
              float m_20, float m_21, float m_22);
    // clang-format on
    transform();

    inline float* data();

    inline float& operator()(std::size_t row, std::size_t col);
    inline float operator()(std::size_t row, std::size_t col) const;

    transform& move(const vec2f& offset);
    vec2f move_point(const vec2f& point);
    transform moved(const vec2f& offset);

    transform& scale(const vec2f& scale);
    vec2f scale_point(const vec2f& point);
    transform scaled(const vec2f& scale);

    using radian = float;
    transform& rotate(radian angle);
    vec2f rotate_point(const vec2f& point);
    transform rotated(radian angle);

    vec2f transform_point(const vec2f& point);
    transform& combine(const transform& other);
    friend transform operator*(const transform& lhs, const transform& rhs);

public:
    inline static constexpr size_t width = 3;
    inline static constexpr size_t height = 3;

private:
    // clang-format off
    float mat[9]{
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
    };
    // clang-format on
};

} // namespace nano

#endif // TRANSOFRMABLE_HPP
