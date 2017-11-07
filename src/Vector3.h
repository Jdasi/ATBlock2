#pragma once

namespace JMath
{

struct Vector3
{
    // ----------------
    // Static variables
    // ----------------
    static const Vector3 zero;
    static const Vector3 one;
    static const Vector3 up;
    static const Vector3 right;
    static const Vector3 forward;

    // ------------
    // Constructors
    // ------------
    Vector3();
    explicit Vector3(const float _x, const float _y, const float _z);
    Vector3(const Vector3& _v);

    // ---------------------
    // Assignment operations
    // ---------------------
    Vector3& operator=(const Vector3 _rhs);

    // -----------
    // Comparisons
    // -----------
    bool operator==(const Vector3& _v) const;
    bool operator!=(const Vector3& _v) const;

    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vector3 operator-() const;

    Vector3 operator+(const Vector3& _rhs) const;
    Vector3 operator-(const Vector3& _rhs) const;
    Vector3 operator*(const float _f) const;
    Vector3 operator/(const float _f) const;

    Vector3& operator+=(const Vector3& _rhs);
    Vector3& operator-=(const Vector3& _rhs);
    Vector3& operator*=(const float _f);
    Vector3& operator/=(const float _f);

    // ----------------
    // Special products
    // ----------------
    float dot(const Vector3& _v) const;
    Vector3 cross(const Vector3& _v) const;
    Vector3 componentProduct(const Vector3& _v);

    // ----------------
    // Other operations
    // ----------------
    float magnitude() const;
    float magnitudeSquared() const;

    void normalize();
    Vector3 normalized() const;

    void clamp(const float _magnitude);
    void setMagnitude(const float _magnitude);
    Vector3 lerp(const Vector3& _v, float _f) const;

    // -----------------
    // Static operations
    // -----------------
    static float distance(const Vector3& _lhs, const Vector3& _rhs);
    static float distanceSquared(const Vector3& _lhs, const Vector3& _rhs);

    static void clamp(Vector3& _v, const float _magnitude);
    static void setMagnitude(Vector3& _v, const float _magnitude);

    static float dot(const Vector3& _lhs, const Vector3& _rhs);
    static Vector3 cross(const Vector3& _lhs, const Vector3& _rhs);
    static Vector3 componentProduct(const Vector3& _lhs, const Vector3& _rhs);

    // ---------
    // Variables
    // ---------
    float x;
    float y;
    float z;

};

} // namespace JMath
