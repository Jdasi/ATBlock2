#pragma once

#include "VBModel.h"

class Square : public VBModel
{
public:
    Square(Renderer* _renderer);
    ~Square() = default;

protected:
    void createMesh(Renderer* _renderer) override;

private:


};
