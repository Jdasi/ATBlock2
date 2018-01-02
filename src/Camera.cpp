#include <iostream>

#include "Camera.h"
#include "InputHandler.h"
#include "JTime.h"
#include "GameData.h"


Camera::Camera(const float _fov, const float _ar, const float _near, const float _far,
    const DirectX::XMFLOAT3& _up, const DirectX::XMFLOAT3& _target)
    : GameObject()
    , proj_mat(DirectX::XMMatrixIdentity())
    , view_mat(DirectX::XMMatrixIdentity())
    , field_of_view(_fov)
    , aspect_ratio(_ar)
    , near_plane(_near)
    , far_plane(_far)
    , up(_up)
    , target(_target)
{
}


void Camera::tick(GameData* _gd)
{
    using namespace DirectX;

    handleInput(_gd);

    if (isMatDirty())
    {
        proj_mat = XMMatrixPerspectiveFovLH(field_of_view, aspect_ratio, near_plane, far_plane);

        XMFLOAT3 current_target = target;

        if (relative_look_at)
        {
            auto& pos = getPos();
            current_target = Float3Add(pos, target);
        }

        XMVECTOR eyev = XMLoadFloat3(&getPos());
        XMVECTOR targetv = XMLoadFloat3(&current_target);
        XMVECTOR upv = XMLoadFloat3(&up);

        view_mat = XMMatrixLookAtLH(eyev, targetv, upv);
    }

    // Base class will clear dirty status.
    GameObject::tick(_gd);
}


void Camera::setTarget(const DirectX::XMFLOAT3& _target)
{
    relative_look_at = false;
    target = _target;
}


void Camera::setRelativeTarget(const DirectX::XMFLOAT3& _target)
{
    relative_look_at = true;
    target = _target;
}


const DirectX::XMMATRIX& Camera::getProjMat() const
{
    return proj_mat;
}


const DirectX::XMMATRIX& Camera::getViewMat() const
{
    return view_mat;
}


void Camera::handleInput(GameData* _gd)
{
    auto pos = getPos();
    float modifier = abs(pos.z);

    float move_speed = 0.5f * modifier;
    float scroll_speed = 1;
    
    if (_gd->input->getAction(GameAction::FORWARD))
    {
        adjustPos(0, move_speed * JTime::getDeltaTime(), 0);
    }
    
    if (_gd->input->getAction(GameAction::BACKWARD))
    {
        adjustPos(0, -(move_speed * JTime::getDeltaTime()), 0);
    }
    
    if (_gd->input->getAction(GameAction::LEFT))
    {
        adjustPos(-(move_speed * JTime::getDeltaTime()), 0, 0);
    }
    
    if (_gd->input->getAction(GameAction::RIGHT))
    {
        adjustPos(move_speed * JTime::getDeltaTime(), 0, 0);
    }

    if (_gd->input->getAction(GameAction::UP))
    {
        adjustPos(0, 0, move_speed * JTime::getDeltaTime());
    }

    if (_gd->input->getAction(GameAction::DOWN))
    {
        adjustPos(0, 0, -(move_speed * JTime::getDeltaTime()));
    }

    int scroll = _gd->input->getMouseDeltaZ();
    if (scroll != 0)
    {
        adjustPos(0, 0, scroll * scroll_speed * JTime::getDeltaTime());
    }

    if (isMatDirty())
    {
        //std::cout << "Camera Position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    }

    pos = getPos();
    if (pos.z > -2)
    {
        setPos(pos.x, pos.y, -2);
    }

}
