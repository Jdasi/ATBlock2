#pragma once

#include "VBModel.h"

/* Specialised VBModel that resembles a square shape.
 */
class Square : public VBModel
{
public:
    Square(Renderer* _renderer);
    ~Square() = default;

protected:
    void createMesh(Renderer* _renderer) override;

private:


};
