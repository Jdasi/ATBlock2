#include "Vector2.h"
#include "JMathHelper.h"


const JMath::Vector2 JMath::Vector2::zero(0, 0);
const JMath::Vector2 JMath::Vector2::one(1, 1);
const JMath::Vector2 JMath::Vector2::up(0, 1);
const JMath::Vector2 JMath::Vector2::right(1, 0);


float JMath::Vector2::distance(const Vector2& _lhs, const Vector2& _rhs)
{
    Vector2 diff = _rhs - _lhs;
    return sqrtf((diff.x * diff.x) + (diff.y * diff.y));
}


float JMath::Vector2::distanceSquared(const Vector2& _lhs, const Vector2& _rhs)
{
    Vector2 diff = _rhs - _lhs;
    return (diff.x * diff.x) + (diff.y * diff.y);
}


void JMath::Vector2::clamp(Vector2& _v, const float _magnitude)
{
    if (_v.magnitude() <= _magnitude)
        return;

    setMagnitude(_v, _magnitude);
}


void JMath::Vector2::setMagnitude(Vector2& _v, const float _magnitude)
{
    _v.normalize();

    _v.x *= _magnitude;
    _v.y *= _magnitude;
}


JMath::Vector2::Vector2()
    : x(0)
    , y(0)
{
}


JMath::Vector2::Vector2(const float _x, const float _y)
    : x(_x)
    , y(_y)
{
}


JMath::Vector2::Vector2(const Vector2& _v)
    : x(_v.x)
    , y(_v.y)
{
}


JMath::Vector2& JMath::Vector2::operator=(const Vector2 _rhs)
{
    this->x = _rhs.x;
    this->y = _rhs.y;

    return *this;
}


bool JMath::Vector2::operator==(const Vector2& _v) const
{
    return (
        x > _v.x - EPSILONF && x < _v.x + EPSILONF &&
        y > _v.y - EPSILONF && y < _v.y + EPSILONF);
}


bool JMath::Vector2::operator!=(const Vector2& _v) const
{
    return (
        x < _v.x - EPSILONF || x > _v.x + EPSILONF ||
        y < _v.y - EPSILONF || y > _v.y + EPSILONF);
}


JMath::Vector2 JMath::Vector2::operator-() const
{
    return Vector2(-x, -y);
}


JMath::Vector2 JMath::Vector2::operator+(const Vector2& _rhs) const
{
    Vector2 vec = *this;

    vec.x += _rhs.x;
    vec.y += _rhs.y;

    return vec;
}


JMath::Vector2 JMath::Vector2::operator-(const Vector2& _rhs) const
{
    Vector2 vec = *this;

    vec.x -= _rhs.x;
    vec.y -= _rhs.y;

    return vec;
}


JMath::Vector2 JMath::Vector2::operator*(const float _f) const
{
    Vector2 vec = *this;

    vec.x *= _f;
    vec.y *= _f;

    return vec;
}


JMath::Vector2 JMath::Vector2::operator/(const float _f) const
{
    Vector2 vec = *this;

    vec.x /= _f;
    vec.y /= _f;

    return vec;
}


JMath::Vector2& JMath::Vector2::operator+=(const Vector2& _rhs)
{
    *this = *this + _rhs;
    return *this;
}


JMath::Vector2& JMath::Vector2::operator-=(const Vector2& _rhs)
{
    *this = *this - _rhs;
    return *this;
}


JMath::Vector2& JMath::Vector2::operator*=(const float _f)
{
    *this = *this * _f;
    return *this;
}


JMath::Vector2& JMath::Vector2::operator/=(const float _f)
{
    *this = *this / _f;
    return *this;
}


float JMath::Vector2::magnitude() const
{
    return sqrtf(x * x + y * y);
}


float JMath::Vector2::magnitudeSquared() const
{
    return x * x + y * y;
}


void JMath::Vector2::normalize()
{
    float mag = 1.0f / magnitude();

    x *= mag;
    y *= mag;
}


JMath::Vector2 JMath::Vector2::normalized() const
{
    float mag = 1.0f / magnitude();
    return Vector2(x * mag, y * mag);
}


void JMath::Vector2::clamp(const float _magnitude)
{
    clamp(*this, _magnitude);
}


void JMath::Vector2::setMagnitude(const float _magnitude)
{
    setMagnitude(*this, _magnitude);
}


JMath::Vector2 JMath::Vector2::lerp(const Vector2& _v, const float _f) const
{
    return Vector2(
        x + (_v.x - x) * _f,
        y + (_v.y - y) * _f);
}
