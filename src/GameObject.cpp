#include "GameObject.h"


GameObject::GameObject()
    : visible(true)
    , mat_dirty(true)
    , world_mat(DirectX::XMMatrixIdentity())
    , scale(1, 1, 1)
    , yaw(0)
    , pitch(0)
    , roll(0)
{
}


void GameObject::tick(GameData* _gd)
{
    using namespace DirectX;

    if (mat_dirty)
    {
        mat_dirty = false;

        XMMATRIX scale_mat = XMMatrixScaling(scale.x, scale.y, scale.z);
        XMMATRIX rotation_mat = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
        XMMATRIX trans_mat = XMMatrixTranslation(position.x, position.y, position.z);

        world_mat = scale_mat * rotation_mat * trans_mat;
    }
}


void GameObject::draw(DrawData* _dd)
{
    // Base class draws nothing.
}


bool GameObject::isVisible() const
{
    return visible;
}


void GameObject::setVisible(const bool _visible)
{
    visible = _visible;
}


const DirectX::XMMATRIX& GameObject::getWorld() const
{
    return world_mat;
}


const DirectX::XMFLOAT3& GameObject::getPos() const
{
    return position;
}


void GameObject::setPos(const DirectX::XMFLOAT3& _pos)
{
    position = _pos;
    mat_dirty = true;
}


void GameObject::setPos(const float _x, const float _y, const float _z)
{
    setPos(DirectX::XMFLOAT3(_x, _y, _z));
}


const DirectX::XMFLOAT3& GameObject::getScale() const
{
    return scale;
}


void GameObject::setScale(const DirectX::XMFLOAT3& _scale)
{
    scale = _scale;
    mat_dirty = true;
}


void GameObject::setScale(const float _x, const float _y, const float _z)
{
    setScale(DirectX::XMFLOAT3(_x, _y, _z));
}


void GameObject::setScale(const float _scale)
{
    float eval = 1 * _scale;
    setScale(eval, eval, eval);
}


float GameObject::getYaw() const
{
    return yaw;
}


void GameObject::setYaw(const float _yaw)
{
    yaw = _yaw;
    mat_dirty = true;
}


float GameObject::getPitch() const
{
    return pitch;
}


void GameObject::setPitch(const float _pitch)
{
    pitch = _pitch;
    mat_dirty = true;
}


float GameObject::getRoll() const
{
    return roll;
}


void GameObject::setRoll(const float _roll)
{
    roll = _roll;
    mat_dirty = true;
}


void GameObject::setYawPitchRoll(const float _yaw, const float _pitch, const float _roll)
{
    setYaw(_yaw);
    setPitch(_pitch);
    setRoll(_roll);
}
