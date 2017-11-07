#pragma once

namespace JMath
{

struct Vector3;

struct Matrix
{
    // ----------------
    // Static variables
    // ----------------
    static const Matrix identity;

    // ------------
    // Constructors
    // ------------
    Matrix();
    Matrix(const Matrix&) = default;
    Matrix(
        float _m11, float _m12, float _m13, float _m14,
        float _m21, float _m22, float _m23, float _m24,
        float _m31, float _m32, float _m33, float _m34,
        float _m41, float _m42, float _m43, float _m44);

    // ---------------------
    // Arithmetic operations
    // ---------------------
    Matrix operator*(const Matrix& _m) const;

    // ----------------
    // Other operations
    // ----------------
    Matrix transpose() const;
    float determinant() const;

    // -----------------
    // Static operations
    // -----------------
    static Matrix lookAt(const Vector3& _pos, const Vector3& _look_target, const Vector3& _up);

    // ---------
    // Variables
    // ---------
    union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };

        float m[4][4];
    };

};

}
