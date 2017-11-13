#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
public:
    Camera(const float _fov, const float _ar, const float _near, const float _far,
        const DirectX::XMFLOAT3& _up = DirectX::Vector3Up, const DirectX::XMFLOAT3& _target = DirectX::Vector3Zero);

    ~Camera() = default;

    virtual void tick(GameData* _gd) override;

    void setTarget(const DirectX::XMFLOAT3& _target);
    void setRelativeTarget(const DirectX::XMFLOAT3& _target);

    const DirectX::XMMATRIX& getProjMat() const;
    const DirectX::XMMATRIX& getViewMat() const;

protected:
    void handleInput(GameData* _gd);

    DirectX::XMMATRIX proj_mat;
    DirectX::XMMATRIX view_mat;

    float aspect_ratio;
    float field_of_view;
    float near_plane;
    float far_plane;

    DirectX::XMFLOAT3 target;
    DirectX::XMFLOAT3 up;

    bool relative_look_at;

};
