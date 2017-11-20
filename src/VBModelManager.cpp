#include "VBModelManager.h"
#include "Renderer.h"


VBModelManager::VBModelManager(Renderer* _renderer)
    : renderer(_renderer)
{
}

std::unique_ptr<Square> VBModelManager::createSquare(const Renderer::ShaderType& _shader_type)
{
    auto model = std::make_unique<Square>(renderer);
    model->setShader(renderer->getShaderData(_shader_type));

    return std::move(model);
}
