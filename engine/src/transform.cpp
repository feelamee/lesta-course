#include <transform.hpp>

#include <vec.hpp>

#include <cmath>
#include <cstring>
#include <type_traits>

namespace nano
{

// clang-format off
transform2D::transform2D(float m_00, float m_01, float m_02,
                     float m_10, float m_11, float m_12,
                     float m_20, float m_21, float m_22)
    : mat{m_00, m_01, m_02,
          m_10, m_11, m_12,
          m_20, m_21, m_22,}
{}
// clang-format on

float*
transform2D::data()
{
    return mat;
}

const float*
transform2D::data() const
{
    return mat;
}

float&
transform2D::operator()(std::size_t row, std::size_t col)
{
    return mat[row * width + col];
}

float
transform2D::operator()(std::size_t row, std::size_t col) const
{
    return mat[row * width + col];
}

// TODO: use multiplication of vectors and stride
transform2D&
transform2D::combine(const transform2D& other)
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

transform2D
operator*(const transform2D& lhs, const transform2D& rhs)
{
    transform2D ret{ lhs };
    ret.combine(rhs);
    return ret;
}

transform2D&
transform2D::move(const vec2f& offset)
{
    // clang-format off
    const transform2D moving = {
                    1, 0, offset.x,
                    0, 1, offset.y,
                    0, 0,          1};
    // clang-format on

    return combine(moving);
}

transform2D
transform2D::moved(const vec2f& offset)
{
    transform2D ret = (*this);
    ret.move(offset);
    return ret;
}

transform2D&
transform2D::scale(const vec2f& scale)
{
    // clang-format off
    const transform2D scaling = {
                    scale.x, 0,         0,
                    0,         scale.y, 0,
                    0,         0,         1};
    // clang-format on

    return combine(scaling);
}

transform2D
transform2D::scaled(const vec2f& scale)
{
    transform2D ret = (*this);
    ret.scale(scale);
    return ret;
}

///////////////////////////////
// vec2f
// move_point(const vec2f& point)
// {
// }

// vec2f
// scale_point(const vec2f& point)
// {
// }

// vec2f
// rotate_point(const vec2f& point)
// {
// }

// vec2f
// transform_point(const vec2f& point)
// {
// }
///////////////////////////////

transform2D&
transform2D::rotate(radian angle)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);

    // clang-format off
    const transform2D rot{cos, -sin, 0,
                        sin,  cos, 0,
                          0,    0, 1};
    // clang-format on

    return combine(rot);
}

transform2D
transform2D::rotated(radian angle)
{
    transform2D ret = (*this);
    ret.rotate(angle);
    return ret;
}

} // namespace nano
