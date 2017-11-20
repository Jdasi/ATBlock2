#pragma once

#include <map>
#include <memory>

#include "Square.h"
#include "Renderer.h"

class VBModelFactory
{
public:
    VBModelFactory(Renderer* _renderer);
    ~VBModelFactory() = default;

    std::unique_ptr<Square> createSquare(const Renderer::ShaderType& _shader_type =
        Renderer::ShaderType::DEFAULT);

private:
    Renderer* renderer;

};
