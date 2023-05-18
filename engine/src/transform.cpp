#include <transform.hpp>

#include <vec.hpp>

#include <cmath>
#include <cstring>
#include <type_traits>

namespace nano
{

// clang-format off
transform::transform(float m_00, float m_01, float m_02,
                     float m_10, float m_11, float m_12,
                     float m_20, float m_21, float m_22)
    : mat{m_00, m_01, m_02,
          m_10, m_11, m_12,
          m_20, m_21, m_22,}
{}
// clang-format on

transform::transform()
    : mat{ 0 }
{
}

inline float*
transform::data()
{
    return mat;
}

inline float&
transform::operator()(std::size_t col_x, std::size_t row_y)
{
    return mat[row_y * width + col_x];
}

inline float
transform::operator()(std::size_t col_x, std::size_t row_y) const
{
    return mat[row_y * width + col_x];
}

// TODO: use multiplication of vectors and stride
transform&
transform::combine(const transform& other)
{

    auto& mat = *this;
    // clang-format off
    mat(0, 0) = mat(0, 0) * other(0, 0) + mat(1, 0) * other(0, 1) + mat(2, 0) * other(0, 2);
    mat(1, 0) = mat(0, 0) * other(1, 0) + mat(1, 0) * other(1, 1) + mat(2, 0) * other(1, 2);
    mat(2, 0) = mat(0, 0) * other(2, 0) + mat(1, 0) * other(2, 1) + mat(2, 0) * other(2, 2);

    mat(0, 1) = mat(0, 1) * other(0, 0) + mat(1, 1) * other(0, 1) + mat(2, 1) * other(0, 2);
    mat(1, 1) = mat(0, 1) * other(1, 0) + mat(1, 1) * other(1, 1) + mat(2, 1) * other(1, 2);
    mat(2, 1) = mat(0, 1) * other(2, 0) + mat(1, 1) * other(2, 1) + mat(2, 1) * other(2, 2);

    mat(0, 2) = mat(0, 2) * other(0, 0) + mat(1, 2) * other(0, 1) + mat(2, 2) * other(0, 2);
    mat(1, 2) = mat(0, 2) * other(1, 0) + mat(1, 2) * other(1, 1) + mat(2, 2) * other(1, 2);
    mat(2, 2) = mat(0, 2) * other(2, 0) + mat(1, 2) * other(2, 1) + mat(2, 2) * other(2, 2);
    // clang-format on

    return *this;
}

transform
operator*(const transform& lhs, const transform& rhs)
{
    transform ret{ lhs };
    ret.combine(rhs);
    return ret;
}

transform&
transform::move(const vec2f& offset)
{
    // clang-format off
    const transform moving = {
                    1, 0, offset.fst,
                    0, 1, offset.snd,
                    0, 0,          1};
    // clang-format on

    return combine(moving);
}

transform
transform::moved(const vec2f& offset)
{
    transform ret = (*this);
    ret.move(offset);
    return ret;
}

transform&
transform::scale(const vec2f& scale)
{
    // clang-format off
    const transform scaling = {
                    scale.fst, 0,         0,
                    0,         scale.snd, 0,
                    0,         0,         1};
    // clang-format on

    return combine(scaling);
}

transform
transform::scaled(const vec2f& scale)
{
    transform ret = (*this);
    ret.scale(scale);
    return ret;
}
///////////////////////////////
vec2f
move_point(const vec2f& point)
{
}

vec2f
scale_point(const vec2f& point)
{
}

vec2f
rotate_point(const vec2f& point)
{
}

vec2f
transform_point(const vec2f& point)
{
}
///////////////////////////////
transform&
transform::rotate(radian angle)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);

    // clang-format off
    const transform rot{cos, -sin, 0,
                        sin,  cos, 0,
                          0,    0, 1};
    // clang-format on

    return combine(rot);
}

transform
transform::rotated(radian angle)
{
    transform ret = (*this);
    ret.rotate(angle);
    return ret;
}

} // namespace nano
