#include "utilities.hpp"

namespace Geom
{

Vec2 &Vec2::operator+=(const Vec2 &rhs)
{
    x_ += rhs.x_; y_ += rhs.y_;
    return *this;
}

Vec2 &Vec2::operator-=(const Vec2 &rhs)
{
    x_ -= rhs.x_; y_ -= rhs.y_;
    return *this;
}

Vec2 &Vec2::operator*=(scalar_t rhs)
{
    x_ *= rhs; y_ *= rhs;
    return *this;
}

bool Vec2::operator==(const Vec2 &rhs) const
{
    return eq(x_, rhs.x_) && eq(y_, rhs.y_);
}

Vec2 Vec2::operator-() const
{
    return Vec2{-x_, -y_};
}


Vec2 Vec2::norm_vec() const
{
    scalar_t len = sqrt(x_*x_ + y_*y_);
    if (is_zero())
        return Vec2{0, 0};
    return Vec2{x_/len, y_/len};
}

scalar_t Vec2::len() const
{
    return sqrt(x_ * x_ + y_ * y_);
}

bool Vec2::is_zero() const
{
    return eq(len(), 0);
}

Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs)
{
    Vec2 tmp{lhs}; tmp += rhs;
    return tmp;
}

Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs)
{
    return lhs + -rhs;
}

Vec2 operator*(scalar_t scalar, const Vec2& vector)
{
    Vec2 tmp{vector}; tmp *= scalar;
    return tmp;
}

Vec2 operator*(const Vec2 &vector, scalar_t scalar)
{
    Vec2 tmp{vector}; tmp *= scalar;
    return tmp;
}

} // namespace Geom