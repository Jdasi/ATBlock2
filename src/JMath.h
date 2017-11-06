#pragma once

#include <limits>

namespace JMath
{

// -------------------
// Namespace Variables
// -------------------
const float EPSILONF = std::numeric_limits<float>::epsilon();

// -------------------
// Namespace functions
// -------------------
inline int clamp(const int _original, const int _min, const int _max)
{
    if (_original < _min) return _min;
    if (_original > _max) return _max;

    return _original;
}


inline float clampf(const float _original, const float _min, const float _max)
{
    if (_original < _min) return _min;
    if (_original > _max) return _max;

    return _original;
}


inline float degToRad(float _f)
{
    return _f * 0.017453293f;
}


inline float radToDeg(float _f)
{
    return _f * 57.29577951f;
}


// -----------------
// Namespace objects
// -----------------
struct Vector2
{
    static const Vector2 zero;
    static const Vector2 up;
    static const Vector2 right;
    static const Vector2 forward;

    // -----------------
    // Static operations
    // -----------------
    static float distance(const Vector2& _lhs, const Vector2& _rhs)
    {
        Vector2 diff = _rhs - _lhs;
        return sqrtf((diff.x * diff.x) + (diff.y * diff.y));
    }


    static float distanceSquared(const Vector2& _lhs, const Vector2& _rhs)
    {
        Vector2 diff = _rhs - _lhs;
        return (diff.x * diff.x) + (diff.y * diff.y);
    }


    static void clamp(Vector2& _v, const float _magnitude)
    {
        if (_v.magnitude() <= _magnitude)
            return;

        setMagnitude(_v, _magnitude);
    }


    static void setMagnitude(Vector2& _v, const float _magnitude)
    {
        _v.normalize();

        _v.x *= _magnitude;
        _v.y *= _magnitude;
    }


    // ------------
    // Constructors
    // ------------
    Vector2()
        : x(0)
        , y(0)
    {}


    explicit Vector2(const float _x, const float _y)
        : x(_x)
        , y(_y)
    {}


    Vector2(const Vector2& _v)
        : x(_v.x)
        , y(_v.y)
    {}


    // ---------------------
    // Assignment operations
    // ---------------------
    Vector2& operator=(const Vector2 _rhs)
    {
        this->x = _rhs.x;
        this->y = _rhs.y;

        return *this;
    }


    // -----------
    // Comparisons
    // -----------
    bool operator==(const Vector2& _v) const
    {
        return (x > _v.x - EPSILONF && x < _v.x + EPSILONF &&
                y > _v.y - EPSILONF && y < _v.y + EPSILONF);
    }


    bool operator!=(const Vector2& _v) const
    {
        return (x < _v.x - EPSILONF || x > _v.x + EPSILONF ||
                y < _v.y - EPSILONF || y > _v.y + EPSILONF);
    }


    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vector2 operator-() const
    {
        return Vector2(-x, -y);
    }


    Vector2 operator+(const Vector2& _rhs) const
    {
        Vector2 vec = *this;

        vec.x += _rhs.x;
        vec.y += _rhs.y;

        return vec;
    }


    Vector2& operator+=(const Vector2& _rhs)
    {
        *this = *this + _rhs;
        return *this;
    }


    Vector2 operator-(const Vector2& _rhs) const
    {
        Vector2 vec = *this;

        vec.x -= _rhs.x;
        vec.y -= _rhs.y;

        return vec;
    }


    Vector2& operator-=(const Vector2& _rhs)
    {
        *this = *this - _rhs;
        return *this;
    }


    Vector2 operator*(const Vector2& _rhs) const
    {
        Vector2 vec = *this;

        vec.x *= _rhs.x;
        vec.y *= _rhs.y;

        return vec;
    }


    Vector2& operator*=(const Vector2& _rhs)
    {
        *this = *this * _rhs;
        return *this;
    }


    Vector2 operator/(const Vector2& _rhs) const
    {
        Vector2 vec = *this;

        vec.x /= _rhs.x;
        vec.y /= _rhs.y;

        return vec;
    }


    Vector2& operator/=(const Vector2& _rhs)
    {
        *this = *this / _rhs;
        return *this;
    }


    // ----------------
    // Other operations
    // ----------------
    float magnitude() const
    {
        return sqrtf(x * x + y * y);
    }


    float magnitudeSquared() const
    {
        return x * x + y * y;
    }


    Vector2 normalized() const
    {
        float mag = 1.0f / magnitude();
        return Vector2(x * mag, y * mag);
    }


    void normalize()
    {
        float mag = 1.0f / magnitude();

        x *= mag;
        y *= mag;
    }


    void clamp(const float _magnitude)
    {
        clamp(*this, _magnitude);
    }


    void setMagnitude(const float _magnitude)
    {
        setMagnitude(*this, _magnitude);
    }


    Vector2 lerp(const Vector2& _v, float _f) const
    {
        return Vector2(x + (_v.x - x) * _f,
                    y + (_v.y - y) * _f);
    }


    float x;
    float y;

};

struct Vector3
{
    static const Vector3 zero;
    static const Vector3 up;
    static const Vector3 right;
    static const Vector3 forward;

    // -----------------
    // Static operations
    // -----------------
    static float distance(const Vector3& _lhs, const Vector3& _rhs)
    {
        Vector3 diff = _rhs - _lhs;
        return sqrtf((diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z));
    }


    static float distanceSquared(const Vector3& _lhs, const Vector3& _rhs)
    {
        Vector3 diff = _rhs - _lhs;
        return (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    }


    static void clamp(Vector3& _v, const float _magnitude)
    {
        if (_v.magnitude() <= _magnitude)
            return;

        setMagnitude(_v, _magnitude);
    }


    static void setMagnitude(Vector3& _v, const float _magnitude)
    {
        _v.normalize();

        _v.x *= _magnitude;
        _v.y *= _magnitude;
        _v.z *= _magnitude;
    }


    // ------------
    // Constructors
    // ------------
    Vector3()
        : x(0)
        , y(0)
        , z(0)
    {}


    explicit Vector3(const float _x, const float _y, const float _z)
        : x(_x)
        , y(_y)
        , z(_z)
    {}


    Vector3(const Vector3& _v)
        : x(_v.x)
        , y(_v.y)
        , z(_v.z)
    {}


    // ---------------------
    // Assignment operations
    // ---------------------
    Vector3& operator=(const Vector3 _rhs)
    {
        this->x = _rhs.x;
        this->y = _rhs.y;
        this->z = _rhs.z;

        return *this;
    }


    // -----------
    // Comparisons
    // -----------
    bool operator==(const Vector3& _v) const
    {
        return (x > _v.x - EPSILONF && x < _v.x + EPSILONF &&
                y > _v.y - EPSILONF && y < _v.y + EPSILONF &&
                z > _v.z - EPSILONF && z < _v.z + EPSILONF);
    }


    bool operator!=(const Vector3& _v) const
    {
        return (x < _v.x - EPSILONF || x > _v.x + EPSILONF ||
                y < _v.y - EPSILONF || y > _v.y + EPSILONF ||
                z < _v.z - EPSILONF || z > _v.z + EPSILONF);
    }


    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vector3 operator-() const
    {
        return Vector3(-x, -y, -z);
    }


    Vector3 operator+(const Vector3& _rhs) const
    {
        Vector3 vec = *this;

        vec.x += _rhs.x;
        vec.y += _rhs.y;
        vec.z += _rhs.z;

        return vec;
    }


    Vector3& operator+=(const Vector3& _rhs)
    {
        *this = *this + _rhs;
        return *this;
    }


    Vector3 operator-(const Vector3& _rhs) const
    {
        Vector3 vec = *this;

        vec.x -= _rhs.x;
        vec.y -= _rhs.y;
        vec.z -= _rhs.z;

        return vec;
    }


    Vector3& operator-=(const Vector3& _rhs)
    {
        *this = *this - _rhs;
        return *this;
    }


    Vector3 operator*(const Vector3& _rhs) const
    {
        Vector3 vec = *this;

        vec.x *= _rhs.x;
        vec.y *= _rhs.y;
        vec.z *= _rhs.z;

        return vec;
    }


    Vector3& operator*=(const Vector3& _rhs)
    {
        *this = *this * _rhs;
        return *this;
    }


    Vector3 operator/(const Vector3& _rhs) const
    {
        Vector3 vec = *this;

        vec.x /= _rhs.x;
        vec.y /= _rhs.y;
        vec.z /= _rhs.z;

        return vec;
    }


    Vector3& operator/=(const Vector3& _rhs)
    {
        *this = *this / _rhs;
        return *this;
    }


    // ----------------
    // Special products
    // ----------------
    float dot(const Vector3& _v) const
    {
        return x * _v.x + y * _v.y + z * _v.z;
    }


    Vector3 cross(const Vector3& _v) const
    {
        return Vector3(y * _v.z - z * _v.y,
                    z * _v.x - x * _v.z,
                    x * _v.y - y * _v.x);
    }


    // ----------------
    // Other operations
    // ----------------
    float magnitude() const
    {
        return sqrtf(x * x + y * y + z * z);
    }


    float magnitudeSquared() const
    {
        return x * x + y * y + z * z;
    }


    Vector3 normalized() const
    {
        float mag = 1.0f / magnitude();
        return Vector3(x * mag, y * mag, z * mag);
    }


    void normalize()
    {
        float mag = 1.0f / magnitude();

        x *= mag;
        y *= mag;
        y *= mag;
    }


    void clamp(const float _magnitude)
    {
        clamp(*this, _magnitude);
    }


    void setMagnitude(const float _magnitude)
    {
        setMagnitude(*this, _magnitude);
    }


    Vector3 lerp(const Vector3& _v, float _f) const
    {
        return Vector3(x + (_v.x - x) * _f,
                    y + (_v.y - y) * _f,
                    z + (_v.z - z) * _f);
    }


    float x;
    float y;
    float z;

};

struct Vector4
{
    // ------------
    // Constructors
    // ------------
    Vector4()
        : x(0)
        , y(0)
        , z(0)
        , w(0)
    {}


    explicit Vector4(const float _x, const float _y, const float _z, const float _w)
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
    {}


    Vector4(const Vector4& _v)
        : x(_v.x)
        , y(_v.y)
        , z(_v.z)
        , w(_v.w)
    {}


    // ---------------------
    // Assignment operations
    // ---------------------
    Vector4& operator=(const Vector4 _rhs)
    {
        this->x = _rhs.x;
        this->y = _rhs.y;
        this->z = _rhs.z;
        this->w = _rhs.w;

        return *this;
    }


    // -----------
    // Comparisons
    // -----------
    bool operator==(const Vector4& _v) const
    {
        return (x > _v.x - EPSILONF && x < _v.x + EPSILONF &&
                y > _v.y - EPSILONF && y < _v.y + EPSILONF &&
                z > _v.z - EPSILONF && z < _v.z + EPSILONF &&
                w > _v.w - EPSILONF && z < _v.w + EPSILONF);
    }


    bool operator!=(const Vector4& _v) const
    {
        return (x < _v.x - EPSILONF || x > _v.x + EPSILONF ||
                y < _v.y - EPSILONF || y > _v.y + EPSILONF ||
                z < _v.z - EPSILONF || z > _v.z + EPSILONF ||
                w < _v.w - EPSILONF || w > _v.w + EPSILONF);
    }


    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vector4 operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }


    Vector4 operator+(const Vector4& _rhs) const
    {
        Vector4 vec = *this;

        vec.x += _rhs.x;
        vec.y += _rhs.y;
        vec.z += _rhs.z;
        vec.w += _rhs.w;

        return vec;
    }


    Vector4& operator+=(const Vector4& _rhs)
    {
        *this = *this + _rhs;
        return *this;
    }


    Vector4 operator-(const Vector4& _rhs) const
    {
        Vector4 vec = *this;

        vec.x -= _rhs.x;
        vec.y -= _rhs.y;
        vec.z -= _rhs.z;
        vec.w -= _rhs.w;

        return vec;
    }


    Vector4& operator-=(const Vector4& _rhs)
    {
        *this = *this - _rhs;
        return *this;
    }


    Vector4 operator*(const Vector4& _rhs) const
    {
        Vector4 vec = *this;

        vec.x *= _rhs.x;
        vec.y *= _rhs.y;
        vec.z *= _rhs.z;
        vec.w *= _rhs.w;

        return vec;
    }


    Vector4& operator*=(const Vector4& _rhs)
    {
        *this = *this * _rhs;
        return *this;
    }


    Vector4 operator/(const Vector4& _rhs) const
    {
        Vector4 vec = *this;

        vec.x /= _rhs.x;
        vec.y /= _rhs.y;
        vec.z /= _rhs.z;
        vec.w /= _rhs.w;

        return vec;
    }


    Vector4& operator/=(const Vector4& _rhs)
    {
        *this = *this / _rhs;
        return *this;
    }


    float x;
    float y;
    float z;
    float w;

};

struct Quaternion
{
    // ------------
    // Constructors
    // ------------
    Quaternion()
        : x(0)
        , y(0)
        , z(0)
        , w(0)
    {}


    explicit Quaternion(const float _x, const float _y, const float _z, const float _w)
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
    {}


    Quaternion(const float _euler_x, const float _euler_y, const float _euler_z)
    {
        Quaternion yaw(0, 0, sinf(_euler_z * 0.5f), cosf(_euler_z * 0.5f));
        Quaternion pitch(0, sinf(_euler_y * 0.5f), 0, cosf(_euler_y * 0.5f));
        Quaternion roll(sinf(_euler_x * 0.5f), 0, 0, cosf(_euler_x * 0.5f));

        // Order: y * x * z
        *this = pitch * roll * yaw;
    }


    // ---------------------
    // Arithmetic operations
    // ---------------------
    Quaternion operator*(const Quaternion& _q) const
    {
        return Quaternion(y * _q.z - z * _q.y + _q.x * w + x * _q.w,
                          z * _q.x - x * _q.z + _q.y * w + y * _q.w,
                          x * _q.y - y * _q.x + _q.z * w + z * _q.w,
                          w * _q.w - (x * _q.x + y * _q.y + z * _q.z));
    }


    Quaternion &operator*=(const Quaternion &q)
    {
        return *this = *this * q;
    }


    Quaternion inverted() const
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


    float x;
    float y;
    float z;
    float w;

};

} // namespace JMath
