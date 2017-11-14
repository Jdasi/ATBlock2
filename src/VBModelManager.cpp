#include "VBModelManager.h"
#include "Renderer.h"


VBModelManager::VBModelManager(Renderer* _renderer)
    : renderer(_renderer)
{
}


VBModel* VBModelManager::getModel(const std::string& _name)
{
    auto entry = vb_models.find(_name);
    if (entry != vb_models.end())
        return entry->second.get();

    return createModelEntry(_name);
}


VBModel* VBModelManager::createModelEntry(const std::string& _file_name)
{
    auto model = std::make_unique<VBModel>(renderer);
    auto model_p = model.get();

    vb_models[_file_name] = std::move(model);

    return model_p;
}
