#include <memory>

#include "Matrix.h"
#include "Vector3.h"


const JMath::Matrix JMath::Matrix::identity =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};


JMath::Matrix::Matrix()
    : _11(0.0f), _12(0.0f), _13(0.0f), _14(0.0f)
    , _21(0.0f), _22(0.0f), _23(0.0f), _24(0.0f)
    , _31(0.0f), _32(0.0f), _33(0.0f), _34(0.0f)
    , _41(0.0f), _42(0.0f), _43(0.0f), _44(0.0f)
{
}


JMath::Matrix::Matrix(
    float _m11, float _m12, float _m13, float _m14,
    float _m21, float _m22, float _m23, float _m24,
    float _m31, float _m32, float _m33, float _m34,
    float _m41, float _m42, float _m43, float _m44)
    :
      _11(_m11), _12(_m12), _13(_m13), _14(_m14)
    , _21(_m21), _22(_m22), _23(_m23), _24(_m24)
    , _31(_m31), _32(_m32), _33(_m33), _34(_m34)
    , _41(_m41), _42(_m42), _43(_m43), _44(_m44)
{
}


JMath::Matrix JMath::Matrix::operator*(const Matrix& _m) const
{
    Matrix mult;

    for (unsigned int row = 0; row < 4; ++row)
    {
        for (unsigned int col = 0; col < 4; ++col)
        {
            mult.m[row][col] = 0.0f;

            for (unsigned int k = 0; k < 4; ++k)
            {
                mult.m[row][col] += m[row][k] * _m.m[k][col];
            }
        }
    }

    return std::move(mult);
}


JMath::Matrix JMath::Matrix::transpose() const
{
    Matrix transposed;

    for (unsigned int row = 0u; row < 4u; ++row)
    {
        for (unsigned int col = 0u; col < 4u; ++col)
        {
            transposed.m[row][col] = m[col][row];
        }
    }

    return transposed;
}


float JMath::Matrix::determinant() const
{
    return
        _11 * (_22 * _33 - _23 * _32) -
        _12 * (_21 * _33 - _13 * _31) +
        _13 * (_21 * _32 - _23 * _31);
}


JMath::Matrix JMath::Matrix::lookAt(const Vector3& _pos, const Vector3& _target, const Vector3& _up)
{
    Vector3 z_axis = (_target - _pos).normalized();
    Vector3 x_axis = Vector3::cross(_up, z_axis).normalized();
    Vector3 y_axis = Vector3::cross(z_axis, x_axis);

    return Matrix(
        x_axis.x, y_axis.x, z_axis.x, 0.f,
        x_axis.y, y_axis.y, z_axis.y, 0.f,
        x_axis.z, y_axis.z, z_axis.z, 0.f,
        -Vector3::dot(x_axis, _pos), -Vector3::dot(y_axis, _pos), -Vector3::dot(z_axis, _pos), 1.f);
}


