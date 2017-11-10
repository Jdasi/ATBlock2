#include "Camera.h"


Camera::Camera(const float _fov, const float _ar, const float _near, const float _far,
    const DirectX::XMFLOAT3 _up, const DirectX::XMFLOAT3& _target)
    : field_of_view(_fov)
    , aspect_ratio(_ar)
    , near_plane(_near)
    , far_plane(_far)
    , up(_up)
    , target(_target)
{
    proj_mat = DirectX::XMMatrixIdentity();
    view_mat = DirectX::XMMatrixIdentity();
}


void Camera::tick(GameData* _gd)
{
    using namespace DirectX;

    if (mat_dirty)
    {
        proj_mat = DirectX::XMMatrixPerspectiveFovLH(field_of_view, aspect_ratio, near_plane, far_plane);

        XMVECTOR eyev = XMLoadFloat3(&position);
        XMVECTOR targetv = XMLoadFloat3(&target);
        XMVECTOR upv = XMLoadFloat3(&up);

        view_mat = XMMatrixLookAtLH(eyev, targetv, upv);
    }

    // Base class will clear dirty status.
    GameObject::tick(_gd);
}


const DirectX::XMMATRIX& Camera::getProjMat() const
{
    return proj_mat;
}


const DirectX::XMMATRIX& Camera::getViewMat() const
{
    return view_mat;
}
