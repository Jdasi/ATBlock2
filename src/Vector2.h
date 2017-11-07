#pragma once

namespace JMath
{

struct Vector2
{
    // ----------------
    // Static variables
    // ----------------
    static const Vector2 zero;
    static const Vector2 one;
    static const Vector2 up;
    static const Vector2 right;

    // ------------
    // Constructors
    // ------------
    Vector2();
    explicit Vector2(const float _x, const float _y);
    Vector2(const Vector2& _v);

    // ---------------------
    // Assignment operations
    // ---------------------
    Vector2& operator=(const Vector2 _rhs);
    
    // -----------
    // Comparisons
    // -----------
    bool operator==(const Vector2& _v) const;
    bool operator!=(const Vector2& _v) const;

    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vector2 operator-() const;

    Vector2 operator+(const Vector2& _rhs) const;
    Vector2 operator-(const Vector2& _rhs) const;
    Vector2 operator*(const float _f) const;
    Vector2 operator/(const float _f) const;

    Vector2& operator+=(const Vector2& _rhs);
    Vector2& operator-=(const Vector2& _rhs);
    Vector2& operator*=(const float _f);
    Vector2& operator/=(const float _f);

    // ----------------
    // Other operations
    // ----------------
    float magnitude() const;
    float magnitudeSquared() const;

    void normalize();
    Vector2 normalized() const;

    void clamp(const float _magnitude);
    void setMagnitude(const float _magnitude);
    Vector2 lerp(const Vector2& _v, const float _f) const;

    // -----------------
    // Static operations
    // -----------------
    static float distance(const Vector2& _lhs, const Vector2& _rhs);
    static float distanceSquared(const Vector2& _lhs, const Vector2& _rhs);

    static void clamp(Vector2& _v, const float _magnitude);
    static void setMagnitude(Vector2& _v, const float _magnitude);

    // ---------
    // Variables
    // ---------
    float x;
    float y;

};

} // namespace JMath
