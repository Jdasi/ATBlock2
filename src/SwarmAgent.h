#pragma once

#include <DirectXMath.h>

class SwarmAgent
{
public:
    SwarmAgent();
    ~SwarmAgent() = default;

    const DirectX::XMFLOAT3& getPos() const;
    void setPos(const DirectX::XMFLOAT3& _pos);
    void setPos(const float _x, const float _y, const float _z);
    void adjustPos(const float _x, const float _y, const float _z);

    const DirectX::XMFLOAT4& getCol() const;
    void setColor(const DirectX::XMFLOAT4& _color);
    void setColor(const float _r, const float _g, const float _b, const float _a);

private:
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT4 color;

};
