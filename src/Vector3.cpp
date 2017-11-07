#include "Vector3.h"
#include "JMathHelper.h"


const JMath::Vector3 JMath::Vector3::zero(0, 0, 0);
const JMath::Vector3 JMath::Vector3::one(1, 1, 1);
const JMath::Vector3 JMath::Vector3::up(0, 1, 0);
const JMath::Vector3 JMath::Vector3::right(1, 0, 0);
const JMath::Vector3 JMath::Vector3::forward(0, 0, 1);


JMath::Vector3::Vector3()
    : x(0)
    , y(0)
    , z(0)
{
}


JMath::Vector3::Vector3(const float _x, const float _y, const float _z)
    : x(_x)
    , y(_y)
    , z(_z)
{
}


JMath::Vector3::Vector3(const Vector3& _v)
    : x(_v.x)
    , y(_v.y)
    , z(_v.z)
{
}


JMath::Vector3& JMath::Vector3::operator=(const Vector3 _rhs)
{
    this->x = _rhs.x;
    this->y = _rhs.y;
    this->z = _rhs.z;

    return *this;
}


bool JMath::Vector3::operator==(const Vector3& _v) const
{
    return (
        x > _v.x - EPSILONF && x < _v.x + EPSILONF &&
        y > _v.y - EPSILONF && y < _v.y + EPSILONF &&
        z > _v.z - EPSILONF && z < _v.z + EPSILONF);
}


bool JMath::Vector3::operator!=(const Vector3& _v) const
{
    return (
        x < _v.x - EPSILONF || x > _v.x + EPSILONF ||
        y < _v.y - EPSILONF || y > _v.y + EPSILONF ||
        z < _v.z - EPSILONF || z > _v.z + EPSILONF);
}


JMath::Vector3 JMath::Vector3::operator-() const
{
    return Vector3(-x, -y, -z);
}


JMath::Vector3 JMath::Vector3::operator+(const Vector3& _rhs) const
{
    Vector3 vec = *this;

    vec.x += _rhs.x;
    vec.y += _rhs.y;
    vec.z += _rhs.z;

    return vec;
}


JMath::Vector3 JMath::Vector3::operator-(const Vector3& _rhs) const
{
    Vector3 vec = *this;

    vec.x -= _rhs.x;
    vec.y -= _rhs.y;
    vec.z -= _rhs.z;

    return vec;
}


JMath::Vector3 JMath::Vector3::operator*(const float _f) const
{
    Vector3 vec = *this;

    vec.x *= _f;
    vec.y *= _f;
    vec.z *= _f;

    return vec;
}


JMath::Vector3 JMath::Vector3::operator/(const float _f) const
{
    Vector3 vec = *this;

    vec.x /= _f;
    vec.y /= _f;
    vec.z /= _f;

    return vec;
}


JMath::Vector3& JMath::Vector3::operator+=(const Vector3& _rhs)
{
    *this = *this + _rhs;
    return *this;
}


JMath::Vector3& JMath::Vector3::operator-=(const Vector3& _rhs)
{
    *this = *this - _rhs;
    return *this;
}


JMath::Vector3& JMath::Vector3::operator*=(const float _f)
{
    *this = *this * _f;
    return *this;
}


JMath::Vector3& JMath::Vector3::operator/=(const float _f)
{
    *this = *this / _f;
    return *this;
}


float JMath::Vector3::dot(const Vector3& _v) const
{
    return dot(*this, _v);
}


JMath::Vector3 JMath::Vector3::cross(const Vector3& _v) const
{
    return cross(*this, _v);
}


JMath::Vector3 JMath::Vector3::componentProduct(const Vector3& _v)
{
    return componentProduct(*this, _v);
}


float JMath::Vector3::magnitude() const
{
    return sqrtf(x * x + y * y + z * z);
}


float JMath::Vector3::magnitudeSquared() const
{
    return x * x + y * y + z * z;
}


void JMath::Vector3::normalize()
{
    float mag = 1.0f / magnitude();

    x *= mag;
    y *= mag;
    y *= mag;
}


JMath::Vector3 JMath::Vector3::normalized() const
{
    float mag = 1.0f / magnitude();
    return Vector3(x * mag, y * mag, z * mag);
}


void JMath::Vector3::clamp(const float _magnitude)
{
    clamp(*this, _magnitude);
}


void JMath::Vector3::setMagnitude(const float _magnitude)
{
    setMagnitude(*this, _magnitude);
}


JMath::Vector3 JMath::Vector3::lerp(const Vector3& _v, float _f) const
{
    return Vector3(
        x + (_v.x - x) * _f,
        y + (_v.y - y) * _f,
        z + (_v.z - z) * _f);
}


float JMath::Vector3::distance(const Vector3& _lhs, const Vector3& _rhs)
{
    Vector3 diff = _rhs - _lhs;
    return sqrtf((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));
}


float JMath::Vector3::distanceSquared(const Vector3& _lhs, const Vector3& _rhs)
{
    Vector3 diff = _rhs - _lhs;
    return (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
}


void JMath::Vector3::clamp(Vector3& _v, const float _magnitude)
{
    if (_v.magnitude() <= _magnitude)
        return;

    setMagnitude(_v, _magnitude);
}


void JMath::Vector3::setMagnitude(Vector3& _v, const float _magnitude)
{
    _v.normalize();

    _v.x *= _magnitude;
    _v.y *= _magnitude;
    _v.z *= _magnitude;
}


float JMath::Vector3::dot(const Vector3& _lhs, const Vector3& _rhs)
{
    return
        _lhs.x * _rhs.x +
        _lhs.y * _rhs.y +
        _lhs.z * _rhs.z;
}


JMath::Vector3 JMath::Vector3::cross(const Vector3& _lhs, const Vector3& _rhs)
{
    return Vector3(
        _lhs.y * _rhs.z - _lhs.z * _rhs.y,
        _lhs.z * _rhs.x - _lhs.x * _rhs.z,
        _lhs.x * _rhs.y - _lhs.y * _rhs.x
    );
}


JMath::Vector3 JMath::Vector3::componentProduct(const Vector3& _lhs, const Vector3& _rhs)
{
    return Vector3(_lhs.x * _rhs.x, _lhs.y * _rhs.y, _lhs.z * _rhs.z);
}
