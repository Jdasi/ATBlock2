#pragma once

#include "GameObject.h"
#include "VBModel.h"

class VBGO : public GameObject
{
public:
    VBGO(VBModel* _model);
    virtual ~VBGO() = default;

    virtual void tick(GameData* _gd) override;
    virtual void draw(DrawData* _dd) override;

    static void init(Renderer* _renderer);
    static void cleanUp();

private:
    VBModel* model;
    Renderer* renderer;

    static ID3D11Buffer* s_cb_gpu;
    static CBPerObject* s_cb_cpu;

};
