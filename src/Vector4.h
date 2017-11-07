#pragma once

namespace JMath
{

struct Vector4
{
    // ------------
    // Constructors
    // ------------
    Vector4();
    explicit Vector4(const float _x, const float _y, const float _z, const float _w);
    Vector4(const Vector4& _v);

    // ---------------------
    // Assignment operations
    // ---------------------
    Vector4& operator=(const Vector4 _rhs);

    // -----------
    // Comparisons
    // -----------
    bool operator==(const Vector4& _v) const;
    bool operator!=(const Vector4& _v) const;

    // ---------------------
    // Arithmetic operations
    // ---------------------
    Vector4 operator-() const;

    Vector4 operator+(const Vector4& _rhs) const;
    Vector4 operator-(const Vector4& _rhs) const;

    Vector4& operator+=(const Vector4& _rhs);
    Vector4& operator-=(const Vector4& _rhs);

    // ---------
    // Variables
    // ---------
    float x;
    float y;
    float z;
    float w;

};

} // namespace JMath
