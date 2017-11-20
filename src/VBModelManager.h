#pragma once

#include <map>
#include <memory>

#include "Square.h"
#include "Renderer.h"

class VBModelManager
{
public:
    VBModelManager(Renderer* _renderer);
    ~VBModelManager() = default;

    std::unique_ptr<Square> createSquare(const Renderer::ShaderType& _shader_type =
        Renderer::ShaderType::DEFAULT);

private:
    Renderer* renderer;

};
