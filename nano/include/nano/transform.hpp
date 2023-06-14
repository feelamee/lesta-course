#ifndef TRANSOFRMABLE_HPP
#define TRANSOFRMABLE_HPP

#include <cstddef>
#include <nano/vec.hpp>

namespace nano
{

class transform2D
{
public:
    /**
     * @brief m_01, where 0 is column and 1 is row (like x and y)
     */
    // clang-format off
    transform2D(float m_00, float m_01, float m_02,
                float m_10, float m_11, float m_12,
                float m_20, float m_21, float m_22);
    // clang-format on
    transform2D() = default;

    float* data();
    const float* data() const;

    float& operator()(std::size_t row, std::size_t col);
    float operator()(std::size_t row, std::size_t col) const;

    transform2D& move(const vec2f& offset);
    transform2D moved(const vec2f& offset);

    transform2D& scale(const vec2f& scale);
    transform2D scaled(const vec2f& scale);

    using radian = float;
    transform2D& rotate(radian angle, const vec2f origin = { 0, 0 });
    transform2D rotated(radian angle, const vec2f origin = { 0, 0 });

    transform2D& combine(const transform2D& other);
    friend transform2D operator*(const transform2D& lhs,
                                 const transform2D& rhs);

public:
    inline static constexpr std::size_t width = 3;
    inline static constexpr std::size_t height = 3;

private:
    // clang-format off
    float mat[9]{
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f,
    };
    // clang-format on
};

} // namespace nano

#endif // TRANSOFRMABLE_HPP
