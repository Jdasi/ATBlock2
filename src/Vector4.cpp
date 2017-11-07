#include "Vector4.h"
#include "JMathHelper.h"


JMath::Vector4::Vector4()
    : x(0)
    , y(0)
    , z(0)
    , w(0)
{
}


JMath::Vector4::Vector4(const float _x, const float _y, const float _z, const float _w)
    : x(_x)
    , y(_y)
    , z(_z)
    , w(_w)
{
}

JMath::Vector4::Vector4(const JMath::Vector4& _v)
    : x(_v.x)
    , y(_v.y)
    , z(_v.z)
    , w(_v.w)
{
}


JMath::Vector4& JMath::Vector4::operator=(const JMath::Vector4 _rhs)
{
    this->x = _rhs.x;
    this->y = _rhs.y;
    this->z = _rhs.z;
    this->w = _rhs.w;

    return *this;
}


bool JMath::Vector4::operator==(const JMath::Vector4& _v) const
{
    return (
        x > _v.x - EPSILONF && x < _v.x + EPSILONF &&
        y > _v.y - EPSILONF && y < _v.y + EPSILONF &&
        z > _v.z - EPSILONF && z < _v.z + EPSILONF &&
        w > _v.w - EPSILONF && z < _v.w + EPSILONF);
}


bool JMath::Vector4::operator!=(const JMath::Vector4& _v) const
{
    return (
        x < _v.x - EPSILONF || x > _v.x + EPSILONF ||
        y < _v.y - EPSILONF || y > _v.y + EPSILONF ||
        z < _v.z - EPSILONF || z > _v.z + EPSILONF ||
        w < _v.w - EPSILONF || w > _v.w + EPSILONF);
}


JMath::Vector4 JMath::Vector4::operator-() const
{
    return JMath::Vector4(-x, -y, -z, -w);
}


JMath::Vector4 JMath::Vector4::operator+(const JMath::Vector4& _rhs) const
{
    JMath::Vector4 vec = *this;

    vec.x += _rhs.x;
    vec.y += _rhs.y;
    vec.z += _rhs.z;
    vec.w += _rhs.w;

    return vec;
}


JMath::Vector4 JMath::Vector4::operator-(const JMath::Vector4& _rhs) const
{
    JMath::Vector4 vec = *this;

    vec.x -= _rhs.x;
    vec.y -= _rhs.y;
    vec.z -= _rhs.z;
    vec.w -= _rhs.w;

    return vec;
}


JMath::Vector4& JMath::Vector4::operator+=(const JMath::Vector4& _rhs)
{
    *this = *this + _rhs;
    return *this;
}


JMath::Vector4& JMath::Vector4::operator-=(const JMath::Vector4& _rhs)
{
    *this = *this - _rhs;
    return *this;
}
