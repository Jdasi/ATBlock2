#pragma once

#include "DXMathHelper.h"

struct GameData;
struct DrawData;

/* Base class for all individual game objects to inherit from.
 * Uses the dirtyable pattern to minimise unecessary matrix recalculations.
 */
class GameObject
{
public:
    GameObject();
    virtual ~GameObject() = default;

    virtual void tick(GameData* _gd);
    virtual void draw(DrawData* _dd);

    bool isMatDirty() const;

    bool isVisible() const;
    void setVisible(const bool _visible);

    const DirectX::XMMATRIX& getWorldMat() const;
    const DirectX::XMMATRIX& getRotMat() const;

    const DirectX::XMFLOAT3& getPos() const;
    void setPos(const DirectX::XMFLOAT3& _pos);
    void setPos(const float _x, const float _y, const float _z);
    void adjustPos(const float _x, const float _y, const float _z);

    const DirectX::XMFLOAT3& getScale() const;
    void setScale(const DirectX::XMFLOAT3& _scale);
    void setScale(const float _x, const float _y, const float _z);
    void setScale(const float _scale);

    float getYaw() const;
    void setYaw(const float _yaw);

    float getPitch() const;
    void setPitch(const float _pitch);

    float getRoll() const;
    void setRoll(const float _roll);

    void setYawPitchRoll(const float _yaw, const float _pitch, const float _roll);

private:
    bool visible;

    DirectX::XMMATRIX world_mat;
    DirectX::XMMATRIX rot_mat;

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 scale;
    
    float yaw;
    float pitch;
    float roll;

    // Prevents unnecessary matrix recalculations.
    bool mat_dirty;

};
