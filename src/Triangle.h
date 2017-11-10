#pragma once

#include "VBGO.h"

class Triangle final : public VBGO
{
public:
    Triangle(Renderer* _renderer);
    ~Triangle() = default;

    void tick(GameData* _gd) override;
    void draw(DrawData* _dd) override;

protected:
    void createMesh(Renderer* _renderer) override;

private:


};
