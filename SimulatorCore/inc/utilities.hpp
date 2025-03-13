#pragma once

#include <cmath>

namespace Geom
{

using scalar_t = double;

const scalar_t DBL_PRECISION = 1e-30;

inline bool eq(scalar_t a, scalar_t b)
{
    return fabs(a - b) < DBL_PRECISION;
}

class Vec2 final
{
private:
    scalar_t x_, y_;
public:
    Vec2(scalar_t x, scalar_t y) : x_(x), y_(y) {};

    scalar_t x() const {return x_;};
    scalar_t y() const {return y_;};

    Vec2& operator+=(const Vec2& rhs);
    Vec2& operator-=(const Vec2& rhs);
    Vec2& operator*=(scalar_t rhs);
    bool operator==(const Vec2& rhs) const;
    Vec2 operator-() const;

    Vec2 norm_vec() const;
    scalar_t len() const;
    bool is_zero() const;
};

Vec2 operator+(const Vec2& lhs, const Vec2& rhs);
Vec2 operator-(const Vec2& lhs, const Vec2& rhs);
Vec2 operator*(scalar_t scalar, const Vec2& vector);
Vec2 operator*(const Vec2& vector, scalar_t scalar);


} // namespace Utils