#pragma once

#include <memory>

#include <d3d11.h>

#include "GameObject.h"
#include "VBModel.h"

struct CBPerObject;

class VBGO : public GameObject
{
public:
    VBGO(std::unique_ptr<VBModel> _model);
    virtual ~VBGO() override = default;

    virtual void tick(GameData* _gd) override;
    virtual void draw(DrawData* _dd) override;

    const DirectX::XMFLOAT4& getColor() const;
    void setColor(const DirectX::XMFLOAT4& _color);
    void setColor(const float _r, const float _g, const float _b, const float _a);
    void setColor(const float _r, const float _g, const float _b);

    static bool init(Renderer* _renderer);
    static void updateConstantBuffers(DrawData* _dd);
    static void cleanUp();

private:
    std::unique_ptr<VBModel> model;

    static ID3D11Buffer* s_cb_gpu;
    static CBPerObject* s_cb_cpu;

    DirectX::XMFLOAT4 color;

};
