#pragma once

#include "VBGO.h"

class Cube final : public VBGO
{
public:
    Cube(Renderer* _renderer);
    ~Cube() = default;

    void tick(GameData* _gd) override;
    void draw(DrawData* _dd) override;

protected:
    void createMesh(Renderer* _renderer) override;

private:


};
