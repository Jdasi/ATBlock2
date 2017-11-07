#pragma once

namespace JMath
{

struct Quaternion
{
    // ------------
    // Constructors
    // ------------
    Quaternion();
    explicit Quaternion(const float _x, const float _y, const float _z, const float _w);
    Quaternion(const float _euler_x, const float _euler_y, const float _euler_z);

    // ---------------------
    // Arithmetic operations
    // ---------------------
    Quaternion operator*(const Quaternion& _q) const;
    Quaternion& operator*=(const Quaternion& q);

    // -----------------
    // Other operations
    // ----------------
    Quaternion inverted() const;

    // ---------
    // Variables
    // ---------
    float x;
    float y;
    float z;
    float w;

};

} // namespace JMath
