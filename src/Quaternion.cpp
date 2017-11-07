#include "Quaternion.h"
#include "JMathHelper.h"


JMath::Quaternion::Quaternion()
    : x(0)
    , y(0)
    , z(0)
    , w(0)
{
}


JMath::Quaternion::Quaternion(const float _x, const float _y, const float _z, const float _w)
    : x(_x)
    , y(_y)
    , z(_z)
    , w(_w)
{
}


JMath::Quaternion::Quaternion(const float _euler_x, const float _euler_y, const float _euler_z)
{
    Quaternion yaw(0, 0, sinf(_euler_z * 0.5f), cosf(_euler_z * 0.5f));
    Quaternion pitch(0, sinf(_euler_y * 0.5f), 0, cosf(_euler_y * 0.5f));
    Quaternion roll(sinf(_euler_x * 0.5f), 0, 0, cosf(_euler_x * 0.5f));

    // Order: y * x * z
    *this = pitch * roll * yaw;
}


JMath::Quaternion JMath::Quaternion::operator*(const Quaternion& _q) const
{
    return Quaternion(
        y * _q.z - z * _q.y + _q.x * w + x * _q.w,
        z * _q.x - x * _q.z + _q.y * w + y * _q.w,
        x * _q.y - y * _q.x + _q.z * w + z * _q.w,
        w * _q.w - (x * _q.x + y * _q.y + z * _q.z));
}

JMath::Quaternion& JMath::Quaternion::operator*=(const Quaternion& q)
{
    return *this = *this * q;
}


JMath::Quaternion JMath::Quaternion::inverted() const
{
    float mag = x * x + y * y + z * z + w * w;

    if (mag > 0)
    {
        float inv_mag = 1.0f / mag;
        return Quaternion(-x * inv_mag, -y * inv_mag, -z * inv_mag, w * inv_mag);
    }
    else
    {
        return Quaternion();
    }
}
